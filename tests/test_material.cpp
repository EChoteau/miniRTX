// Fichier : test_material.cpp
// Rôle : squelette des tests des paramètres de matériau et de Phong.
// Responsable : Personne B
// Dépendances : Material.hpp, GoogleTest
// TODO : TODO(B): tester contribution ambiante, diffuse, spéculaire et réflexion.
// Cas : résultat ambiant/diffus (0.9,0,0), lumière sous la surface ambiant seul,
// spéculaire (1.4,0.5,0.5), objets d'ombre devant/derrière/sous la lampe.

#include <gtest/gtest.h>
#include <memory>
#include "material/Material.hpp"
#include "geometry/Sphere.hpp"
#include "scene/Scene.hpp"
#include "geometry/HitRecord.hpp"

TEST(MaterialTest, Placeholder) {
    EXPECT_DOUBLE_EQ(rt::Material{}.reflectivity, 0.0);
}

namespace {

rt::Scene sceneWithSphereAt(const rt::Vec3& center) {
    rt::Scene scene;
    auto material = std::make_shared<rt::Material>();
    // La sphère sert uniquement d'obstacle pour le rayon d'ombre.
    scene.addObject(std::make_unique<rt::Sphere>(center, 1.0, material));
    return scene;
}

rt::HitRecord hitWithMaterial(const std::shared_ptr<rt::Material>& material) {
    rt::HitRecord hit;
    hit.point = rt::Vec3(0.0, 0.0, 0.0);
    hit.normal = rt::Vec3(0.0, 1.0, 0.0);
    hit.material = material.get();
    return hit;
}

} // namespace

TEST(MaterialTest, ShadowedWhenObjectIsBetweenPointAndLight) {
    const rt::Scene scene = sceneWithSphereAt(rt::Vec3(0.0, 0.0, 2.0));
    const rt::PointLight light{rt::Vec3(0.0, 0.0, 5.0)};

    // Le rayon rencontre la sphère avant d'atteindre la lumière.
    EXPECT_TRUE(rt::isInShadow(rt::Vec3(0.0, 0.0, 0.0), light, scene));
}

TEST(MaterialTest, NotShadowedByObjectBehindLight) {
    const rt::Scene scene = sceneWithSphereAt(rt::Vec3(0.0, 0.0, 7.0));
    const rt::PointLight light{rt::Vec3(0.0, 0.0, 5.0)};

    // L'obstacle est après la lumière et ne peut donc pas créer cette ombre.
    EXPECT_FALSE(rt::isInShadow(rt::Vec3(0.0, 0.0, 0.0), light, scene));
}

TEST(MaterialTest, NotShadowedByObjectOutsideLightPath) {
    const rt::Scene scene = sceneWithSphereAt(rt::Vec3(2.0, 0.0, 2.0));
    const rt::PointLight light{rt::Vec3(0.0, 0.0, 5.0)};

    // La sphère n'est pas sur la ligne reliant le point à la lumière.
    EXPECT_FALSE(rt::isInShadow(rt::Vec3(0.0, 0.0, 0.0), light, scene));
}

TEST(MaterialTest, AmbientAndDiffuseAreComputedWithoutClamping) {
    auto material = std::make_shared<rt::Material>();
    material->albedo = rt::Color(1.0, 0.0, 0.0);
    material->ambient = 0.1;
    material->diffuse = 0.8;
    material->specular = 0.0;

    rt::Scene scene;
    scene.addLight(rt::PointLight{rt::Vec3(0.0, 10.0, 0.0)});
    const rt::HitRecord hit = hitWithMaterial(material);
    const rt::Color result =
        rt::computeLocalShading(hit, rt::Ray(rt::Vec3(0.0, 1.0, 0.0),
                                             rt::Vec3(0.0, 1.0, 0.0)),
                                scene);

    EXPECT_DOUBLE_EQ(result.x(), 0.9);
    EXPECT_DOUBLE_EQ(result.y(), 0.0);
    EXPECT_DOUBLE_EQ(result.z(), 0.0);
}

TEST(MaterialTest, LightBehindSurfaceOnlyAddsAmbient) {
    auto material = std::make_shared<rt::Material>();
    material->albedo = rt::Color(1.0, 0.0, 0.0);
    material->ambient = 0.1;
    material->diffuse = 0.8;
    material->specular = 0.5;

    rt::Scene scene;
    scene.addLight(rt::PointLight{rt::Vec3(0.0, -10.0, 0.0)});
    const rt::HitRecord hit = hitWithMaterial(material);
    const rt::Color result =
        rt::computeLocalShading(hit, rt::Ray(rt::Vec3(0.0, 1.0, 0.0),
                                             rt::Vec3(0.0, 1.0, 0.0)),
                                scene);

    EXPECT_DOUBLE_EQ(result.x(), 0.1);
    EXPECT_DOUBLE_EQ(result.y(), 0.0);
    EXPECT_DOUBLE_EQ(result.z(), 0.0);
}

TEST(MaterialTest, SpecularContributionCanExceedOne) {
    auto material = std::make_shared<rt::Material>();
    material->albedo = rt::Color(1.0, 0.0, 0.0);
    material->ambient = 0.1;
    material->diffuse = 0.8;
    material->specular = 0.5;
    material->shininess = 1.0;

    rt::Scene scene;
    scene.addLight(rt::PointLight{rt::Vec3(0.0, 10.0, 0.0)});
    const rt::HitRecord hit = hitWithMaterial(material);
    const rt::Color result =
        rt::computeLocalShading(hit, rt::Ray(rt::Vec3(0.0, 1.0, 0.0),
                                             rt::Vec3(0.0, -1.0, 0.0)),
                                scene);

    EXPECT_DOUBLE_EQ(result.x(), 1.4);
    EXPECT_DOUBLE_EQ(result.y(), 0.5);
    EXPECT_DOUBLE_EQ(result.z(), 0.5);
}
