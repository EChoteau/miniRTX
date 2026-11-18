// Fichier : test_render.cpp
// Rôle : tests de bout en bout (scène -> Renderer -> Image -> PPM).
// Responsable : équipe (intégration A + B + C)
// Dépendances : Renderer.hpp, SceneParser.hpp, Sphere.hpp, Plane.hpp, GoogleTest
// Idée : on ne compare pas à une image de référence (fragile), on vérifie des
// propriétés qu'une image correcte doit forcément avoir : ciel bleu en haut,
// sphère au centre, ombre présente, reflets dépendant de maxDepth, rendu
// identique quel que soit le nombre de threads, fichier PPM bien formé.

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "geometry/Plane.hpp"
#include "geometry/Sphere.hpp"
#include "render/Renderer.hpp"
#include "scene/SceneParser.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path kSourceDir{RT_SOURCE_DIR};

rt::RenderSettings smallSettings(int w = 80, int h = 45, int spp = 1, int depth = 5,
                                 unsigned threads = 1) {
    rt::RenderSettings s;
    s.width = w;
    s.height = h;
    s.samplesPerPixel = spp;
    s.maxDepth = depth;
    s.threadCount = threads;
    return s;
}

std::shared_ptr<rt::Material> makeMaterial(const std::string& name, const rt::Color& albedo,
                                           double reflectivity = 0.0) {
    auto m = std::make_shared<rt::Material>();
    m->name = name;
    m->albedo = albedo;
    m->ambient = 0.1;
    m->diffuse = 0.8;
    m->specular = 0.2;
    m->shininess = 32.0;
    m->reflectivity = reflectivity;
    return m;
}

/// Scène vide (aucun objet) : seul le ciel doit apparaître.
rt::Scene skyScene() {
    rt::Scene scene;
    scene.setCameraParams({rt::Vec3(0, 0, 5), rt::Vec3(0, 0, 0), rt::Vec3(0, 1, 0), 60.0});
    scene.setBackground(rt::Color(0.3, 0.5, 1.0));
    return scene;
}

/// Une sphère rouge de rayon 1 au centre de l'image.
rt::Scene redSphereScene() {
    rt::Scene scene = skyScene();
    scene.setCameraParams({rt::Vec3(0, 0, 5), rt::Vec3(0, 0, 0), rt::Vec3(0, 1, 0), 45.0});
    auto red = makeMaterial("red", rt::Color(0.9, 0.1, 0.1));
    scene.addMaterial(red);
    scene.addLight({rt::Vec3(-5, 5, 5), rt::Color(1, 1, 1), 1.0});
    scene.addObject(std::make_unique<rt::Sphere>(rt::Vec3(0, 0, 0), 1.0, red));
    return scene;
}

/// Sol vu de face ; si withOccluder, une sphère HORS du champ de la caméra
/// est placée entre la lumière et le sol : seule son ombre doit changer l'image.
rt::Scene floorScene(bool withOccluder) {
    rt::Scene scene = skyScene();
    scene.setCameraParams({rt::Vec3(0, 2, 5), rt::Vec3(0, 0, 0), rt::Vec3(0, 1, 0), 45.0});
    auto floor = makeMaterial("floor", rt::Color(0.8, 0.8, 0.8));
    scene.addMaterial(floor);
    scene.addLight({rt::Vec3(0, 10, 0), rt::Color(1, 1, 1), 1.0});
    scene.addObject(std::make_unique<rt::Plane>(rt::Vec3(0, 0, 0), rt::Vec3(0, 1, 0), floor));
    if (withOccluder)
        scene.addObject(std::make_unique<rt::Sphere>(rt::Vec3(0, 6, 0), 1.0, floor));
    return scene;
}

/// Sphère miroir entre une sphère rouge et une bleue, sur un sol.
rt::Scene mirrorScene() {
    rt::Scene scene = skyScene();
    scene.setCameraParams({rt::Vec3(0, 1.5, 7), rt::Vec3(0, 1, 0), rt::Vec3(0, 1, 0), 45.0});
    auto floor = makeMaterial("floor", rt::Color(0.5, 0.5, 0.5));
    auto mirror = makeMaterial("mirror", rt::Color(0.95, 0.95, 0.95), 0.95);
    auto red = makeMaterial("red", rt::Color(0.9, 0.1, 0.1));
    auto blue = makeMaterial("blue", rt::Color(0.1, 0.2, 0.9));
    scene.addLight({rt::Vec3(3, 6, 6), rt::Color(1, 1, 1), 1.0});
    scene.addObject(std::make_unique<rt::Plane>(rt::Vec3(0, 0, 0), rt::Vec3(0, 1, 0), floor));
    scene.addObject(std::make_unique<rt::Sphere>(rt::Vec3(0, 1, 0), 1.0, mirror));
    scene.addObject(std::make_unique<rt::Sphere>(rt::Vec3(-2.3, 0.7, 1.2), 0.7, red));
    scene.addObject(std::make_unique<rt::Sphere>(rt::Vec3(2.3, 0.7, 1.2), 0.7, blue));
    return scene;
}

bool isFinite(const rt::Color& c) {
    return std::isfinite(c.x()) && std::isfinite(c.y()) && std::isfinite(c.z());
}

double luminance(const rt::Color& c) { return 0.2126 * c.x() + 0.7152 * c.y() + 0.0722 * c.z(); }

/// Nombre de pixels qui diffèrent de plus de tol (sur au moins un canal).
int countDifferentPixels(const rt::Image& a, const rt::Image& b, double tol = 1e-9) {
    int n = 0;
    for (int y = 0; y < a.height(); ++y)
        for (int x = 0; x < a.width(); ++x) {
            const rt::Color d = a.pixel(x, y) - b.pixel(x, y);
            if (std::abs(d.x()) > tol || std::abs(d.y()) > tol || std::abs(d.z()) > tol) ++n;
        }
    return n;
}

void expectAllFinite(const rt::Image& img) {
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            ASSERT_TRUE(isFinite(img.pixel(x, y))) << "pixel (" << x << "," << y << ") NaN/inf";
}

std::vector<unsigned char> readBinary(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

} // namespace

// ---------------------------------------------------------------- M1 : ciel

TEST(RenderTest, EmptySceneIsSkyGradientBlueOnTop) {
    const rt::Image img = rt::Renderer(smallSettings()).render(skyScene());
    expectAllFinite(img);
    const int cx = img.width() / 2;
    const rt::Color top = img.pixel(cx, 0);
    const rt::Color bottom = img.pixel(cx, img.height() - 1);
    // Dégradé blanc -> bleu : le rouge diminue en montant, le bleu reste fort.
    EXPECT_LT(top.x(), bottom.x());
    EXPECT_GT(top.z(), top.x());
    // Monotone sur la colonne centrale.
    for (int y = 1; y < img.height(); ++y)
        EXPECT_LE(img.pixel(cx, y - 1).x(), img.pixel(cx, y).x() + 1e-12) << "y=" << y;
}

TEST(RenderTest, EveryPixelIsWrittenEvenWithPartialTiles) {
    // 37x23 avec tuiles de 16 : la dernière ligne/colonne de tuiles est partielle.
    rt::RenderSettings s = smallSettings(37, 23, 1, 5, 3);
    s.tileSize = 16;
    const rt::Image img = rt::Renderer(s).render(skyScene());
    ASSERT_EQ(img.width(), 37);
    ASSERT_EQ(img.height(), 23);
    // Le ciel n'est jamais noir : un pixel noir = pixel oublié par le tuilage.
    for (int y = 0; y < img.height(); ++y)
        for (int x = 0; x < img.width(); ++x)
            EXPECT_GT(luminance(img.pixel(x, y)), 0.1) << "pixel (" << x << "," << y << ")";
}

// ---------------------------------------------------------- M2 : sphère

TEST(RenderTest, SphereIsVisibleInCenterAndSkyInCorners) {
    const rt::Image img = rt::Renderer(smallSettings()).render(redSphereScene());
    expectAllFinite(img);
    const rt::Color center = img.pixel(img.width() / 2, img.height() / 2);
    EXPECT_GT(center.x(), 2.0 * center.y()) << "le centre devrait être rouge";
    EXPECT_GT(center.x(), 2.0 * center.z());
    for (auto [x, y] : {std::pair{0, 0}, {img.width() - 1, 0}, {0, img.height() - 1},
                        {img.width() - 1, img.height() - 1}}) {
        const rt::Color c = img.pixel(x, y);
        EXPECT_GE(c.z(), c.x()) << "coin (" << x << "," << y << ") devrait être du ciel";
    }
}

TEST(RenderTest, SphereIsLitOnTheLightSide) {
    // Lumière en haut à gauche : le haut-gauche de la sphère est plus clair que le bas-droit.
    const rt::Image img = rt::Renderer(smallSettings(160, 90)).render(redSphereScene());
    const int cx = img.width() / 2, cy = img.height() / 2;
    const int d = img.height() / 8;  // reste à l'intérieur du disque
    EXPECT_GT(luminance(img.pixel(cx - d, cy - d)), luminance(img.pixel(cx + d, cy + d)));
}

// ------------------------------------------------------------ M3 : ombres

TEST(RenderTest, OccluderOutsideViewOnlyAddsAShadow) {
    const rt::Renderer renderer(smallSettings(160, 90));
    const rt::Image lit = renderer.render(floorScene(false));
    const rt::Image shadowed = renderer.render(floorScene(true));
    expectAllFinite(shadowed);

    int darker = 0, brighter = 0;
    for (int y = 0; y < lit.height(); ++y)
        for (int x = 0; x < lit.width(); ++x) {
            const double dl = luminance(shadowed.pixel(x, y)) - luminance(lit.pixel(x, y));
            if (dl < -1e-6) ++darker;
            if (dl > 1e-6) ++brighter;
        }
    EXPECT_GT(darker, 50) << "aucune ombre visible sur le sol";
    EXPECT_EQ(brighter, 0) << "un objet ne doit jamais éclaircir le sol";
    // L'ombre tombe sous la lumière, donc au centre de l'image.
    const int cx = lit.width() / 2;
    int cy = -1;
    for (int y = lit.height() - 1; y >= 0 && cy < 0; --y)
        if (luminance(shadowed.pixel(cx, y)) < luminance(lit.pixel(cx, y)) - 1e-6) cy = y;
    EXPECT_GE(cy, 0) << "l'ombre devrait couper la colonne centrale";
}

// --------------------------------------------------------- M4 : réflexions

TEST(RenderTest, MirrorDependsOnMaxDepth) {
    const rt::Image d1 = rt::Renderer(smallSettings(160, 90, 1, 1)).render(mirrorScene());
    const rt::Image d5 = rt::Renderer(smallSettings(160, 90, 1, 5)).render(mirrorScene());
    expectAllFinite(d5);
    // Avec plus de rebonds, le miroir montre la scène : l'image change.
    EXPECT_GT(countDifferentPixels(d1, d5), 100);
}

TEST(RenderTest, MirrorShowsRedAndBlueNeighbours) {
    const rt::Image img = rt::Renderer(smallSettings(320, 180, 1, 5)).render(mirrorScene());
    // On cherche dans le disque de la sphère miroir (centre de l'image, un peu au-dessus).
    bool redSeen = false, blueSeen = false;
    const int cx = img.width() / 2, cy = img.height() / 2;
    const int r = img.height() / 6;
    for (int y = cy - r; y <= cy + r; ++y)
        for (int x = cx - r; x <= cx + r; ++x) {
            const rt::Color c = img.pixel(x, y);
            if (c.x() > 0.3 && c.x() > 2.5 * c.y() && c.x() > 2.5 * c.z()) redSeen = true;
            if (c.z() > 0.3 && c.z() > 2.5 * c.x() && c.z() > 1.5 * c.y()) blueSeen = true;
        }
    EXPECT_TRUE(redSeen) << "le reflet de la sphère rouge n'apparaît pas dans le miroir";
    EXPECT_TRUE(blueSeen) << "le reflet de la sphère bleue n'apparaît pas dans le miroir";
}

TEST(RenderTest, MaxDepthHasNoEffectWithoutReflectiveMaterial) {
    const rt::Image d1 = rt::Renderer(smallSettings(80, 45, 1, 1)).render(redSphereScene());
    const rt::Image d8 = rt::Renderer(smallSettings(80, 45, 1, 8)).render(redSphereScene());
    EXPECT_EQ(countDifferentPixels(d1, d8), 0);
}

// ----------------------------------------------- M5 : multithread / AA

TEST(RenderTest, ImageIsIdenticalWhateverTheThreadCount) {
    const rt::Scene scene = mirrorScene();
    const rt::Image t1 = rt::Renderer(smallSettings(96, 54, 4, 5, 1)).render(scene);
    const rt::Image t4 = rt::Renderer(smallSettings(96, 54, 4, 5, 4)).render(scene);
    const rt::Image t7 = rt::Renderer(smallSettings(96, 54, 4, 5, 7)).render(scene);
    EXPECT_EQ(countDifferentPixels(t1, t4, 0.0), 0);
    EXPECT_EQ(countDifferentPixels(t1, t7, 0.0), 0);
}

TEST(RenderTest, AntiAliasingSmoothsSphereEdge) {
    // Avec plusieurs échantillons, un pixel du bord mélange ciel et sphère :
    // on doit trouver des teintes intermédiaires absentes en 1 spp.
    const rt::Image aa = rt::Renderer(smallSettings(80, 45, 16)).render(redSphereScene());
    const rt::Image noAa = rt::Renderer(smallSettings(80, 45, 1)).render(redSphereScene());
    EXPECT_GT(countDifferentPixels(aa, noAa, 1e-3), 0);
    expectAllFinite(aa);
}

// ------------------------------------------- Intégration fichiers .scene

TEST(RenderTest, DemoSceneRendersAndSavesValidPpm) {
    const rt::Scene scene = rt::SceneParser::parseFile((kSourceDir / "scenes/demo.scene").string());
    const rt::Image img = rt::Renderer(smallSettings(64, 36, 2, 5, 2)).render(scene);
    expectAllFinite(img);

    const fs::path out = fs::temp_directory_path() / "miniRTX_render_demo.ppm";
    img.savePPM(out.string());
    const std::vector<unsigned char> bytes = readBinary(out);
    const std::string header = "P6\n64 36\n255\n";
    ASSERT_EQ(bytes.size(), header.size() + 64u * 36u * 3u);
    EXPECT_EQ(std::string(bytes.begin(), bytes.begin() + header.size()), header);

    // Ordre des lignes : la 1re ligne du fichier est le HAUT de l'image (ciel).
    // Le premier pixel (haut gauche) doit être plus bleu que rouge.
    const std::size_t p = header.size();
    EXPECT_GT(bytes[p + 2], bytes[p + 0]);
    std::error_code ec;
    fs::remove(out, ec);
}

TEST(RenderTest, AllTestScenesParseAndRender) {
    const fs::path dir = kSourceDir / "scenes/tests";
    ASSERT_TRUE(fs::is_directory(dir)) << dir;
    int count = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (entry.path().extension() != ".scene") continue;
        SCOPED_TRACE(entry.path().filename().string());
        rt::Scene scene;
        ASSERT_NO_THROW(scene = rt::SceneParser::parseFile(entry.path().string()));
        rt::Image img(1, 1);
        ASSERT_NO_THROW(img = rt::Renderer(smallSettings(48, 27, 1, 5, 2)).render(scene));
        expectAllFinite(img);
        ++count;
    }
    EXPECT_GE(count, 8);
}
