// Fichier : Renderer.cpp
// Rôle : Boucle de rendu, trace récursif et tuilage atomique.
// Responsable : Personne C
// Dépendances : Renderer.hpp, ThreadPool.hpp, Material.hpp
// CHAÎNE DESCENDANTE : main -> SceneParser -> render -> Camera -> trace.
// CHAÎNE REMONTANTE : trace -> Scene::hit -> Material -> Image -> savePPM.

#include "render/Renderer.hpp"
#include "render/ThreadPool.hpp"
#include "material/Material.hpp"

#include <algorithm>
#include <atomic>
#include <limits>
#include <random>
#include <stdexcept>

namespace rt {

namespace {
// Graine de base : chaque tuile utilise baseSeed + tileIndex.
constexpr unsigned kBaseSeed = 1337u;
}

Renderer::Renderer(RenderSettings settings) : settings_(settings) {
    if (settings_.width <= 0 || settings_.height <= 0)
        throw std::invalid_argument("Width and height must be positive");
    if (settings_.samplesPerPixel <= 0)
        throw std::invalid_argument("Samples per pixel must be positive");
    if (settings_.maxDepth < 0)
        throw std::invalid_argument("Max depth must be non-negative");
    if (settings_.tileSize <= 0)
        throw std::invalid_argument("Tile size must be positive");
    if (settings_.threadCount == 0)
        throw std::invalid_argument("Thread count must be at least 1");
}

Image Renderer::render(const Scene& scene) const {
    // Caméra construite une seule fois.
    const CameraParams& cp = scene.cameraParams();
    const double aspect = static_cast<double>(settings_.width) /
                          static_cast<double>(settings_.height);
    const Camera camera(cp.position, cp.lookAt, cp.up, cp.verticalFovDegrees, aspect);

    Image image(settings_.width, settings_.height);

    const int ts = settings_.tileSize;
    const int tilesX = (settings_.width + ts - 1) / ts;
    const int tilesY = (settings_.height + ts - 1) / ts;
    const int tileCount = tilesX * tilesY;
    const int spp = settings_.samplesPerPixel;

    // Rend une tuile : RNG propre à la tuile, donc indépendant du thread qui
    // la traite. Chaque tuile écrit des pixels distincts : pas de mutex.
    auto renderTile = [&](int tileIndex) {
        std::mt19937 rng(kBaseSeed + static_cast<unsigned>(tileIndex));
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        const int x0 = (tileIndex % tilesX) * ts;
        const int y0 = (tileIndex / tilesX) * ts;
        const int x1 = std::min(x0 + ts, settings_.width);
        const int y1 = std::min(y0 + ts, settings_.height);

        for (int y = y0; y < y1; ++y) {
            for (int x = x0; x < x1; ++x) {
                Color sum{};
                for (int s = 0; s < spp; ++s) {
                    // 1 sample : centre du pixel ; sinon position jitterée.
                    const double jx = (spp == 1) ? 0.5 : dist(rng);
                    const double jy = (spp == 1) ? 0.5 : dist(rng);
                    const double u = (x + jx) / settings_.width;
                    const double v = 1.0 - (y + jy) / settings_.height;
                    sum += trace(camera.getRay(u, v), scene, 0);
                }
                // Moyenne linéaire : ni clamp ni gamma (fait dans savePPM).
                image.setPixel(x, y, sum / static_cast<double>(spp));
            }
        }
    };

    // Tâche exécutée par chaque worker : prend les tuiles une à une.
    std::atomic<int> nextTile{0};
    std::atomic<bool> failed{false};

    auto task = [&]() {
        for (;;) {
            if (failed.load()) break;
            const int i = nextTile.fetch_add(1);
            if (i >= tileCount) break;
            try {
                renderTile(i);
            } catch (...) {
                failed.store(true);  // arrête les autres workers plus tôt
                throw;               // ThreadPool capture et relance après join
            }
        }
    };

    ThreadPool pool(settings_.threadCount);
    pool.run(task);
    return image;
}

// TODO(B): Check si l'intégration du shading Phong, ombre et réflexion jusqu'à maxDepth est réalisée
Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {
    if (depth >= settings_.maxDepth) return skyColor(ray, scene);

    HitRecord hit;
    if (!scene.hit(ray, RAY_T_MIN, std::numeric_limits<double>::infinity(), hit))
        return skyColor(ray, scene);

    const ShadingTerms local = computeShadingTerms(hit, ray, scene);

    const double r = hit.material ? hit.material->reflectivity : 0.0;
    if (r > 0.0) {
        const Vec3 dir =
            Vec3::reflect(ray.direction().normalized(), hit.normal).normalized();
        const Ray reflected(hit.point, dir);
        const Color refl = trace(reflected, scene, depth + 1);
        // Seule la couleur propre de la surface s'efface devant le miroir ; le
        // spéculaire (reflet de la lampe) reste entier.
        return local.ambientDiffuse * (1.0 - r) + refl * r + local.specular;
    }

    return local.ambientDiffuse + local.specular;
}

Color Renderer::skyColor(const Ray& ray, const Scene& scene) const {
    const double t = 0.5 * (ray.direction().normalized().y() + 1.0);
    return Color{1.0, 1.0, 1.0} * (1.0 - t) + scene.background() * t;
}

} // namespace rt