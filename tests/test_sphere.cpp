// Fichier : test_sphere.cpp
// Rôle : tests d'intersection rayon-sphère.
// Responsable : Personne A
// Dépendances : Sphere.hpp, GoogleTest

#include <gtest/gtest.h>
#include <cmath>
#include <memory>
#include <stdexcept>
#include "geometry/Sphere.hpp"

using rt::HitRecord;
using rt::Ray;
using rt::Sphere;
using rt::Vec3;

namespace {
std::shared_ptr<rt::Material> mat() { return std::make_shared<rt::Material>(); }
constexpr double INF = rt::INFINITY_VALUE;

void expectVecNear(const Vec3& a, const Vec3& b, double tol = 1e-9) {
    EXPECT_NEAR(a.x(), b.x(), tol);
    EXPECT_NEAR(a.y(), b.y(), tol);
    EXPECT_NEAR(a.z(), b.z(), tol);
}
} // namespace

TEST(SphereTest, Accessors) {
    const Sphere s(Vec3(1, 2, 3), 0.5, mat());
    expectVecNear(s.center(), Vec3(1, 2, 3));
    EXPECT_DOUBLE_EQ(s.radius(), 0.5);
}

TEST(SphereTest, HitAlongAxis) {
    auto m = mat();
    const Sphere s(Vec3(0, 0, -5), 1.0, m);
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 4.0, 1e-9);
    expectVecNear(rec.point, Vec3(0, 0, -4));
    expectVecNear(rec.normal, Vec3(0, 0, 1));
    EXPECT_TRUE(rec.frontFace);
    EXPECT_EQ(rec.material, m.get());
}

TEST(SphereTest, MissAbove) {
    const Sphere s(Vec3(0, 0, -5), 1.0, mat());
    HitRecord rec;
    EXPECT_FALSE(s.hit(Ray(Vec3(0, 2, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
}

TEST(SphereTest, TangentIsHit) {
    const Sphere s(Vec3(0, 0, -5), 1.0, mat());
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0, 1, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 5.0, 1e-9);
    // Au point tangent, la normale est perpendiculaire au rayon : son sens est
    // arbitraire (±y), seule la direction compte.
    EXPECT_NEAR(std::abs(rec.normal.y()), 1.0, 1e-9);
}

TEST(SphereTest, TMaxTooSmall) {
    const Sphere s(Vec3(0, 0, -5), 1.0, mat());
    HitRecord rec;
    EXPECT_FALSE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0.0, 3.0, rec));
}

TEST(SphereTest, SphereBehindRay) {
    const Sphere s(Vec3(0, 0, 5), 1.0, mat());
    HitRecord rec;
    EXPECT_FALSE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
}

TEST(SphereTest, FromInsideUsesFarRoot) {
    const Sphere s(Vec3(0, 0, 0), 1.0, mat());
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, 1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 1.0, 1e-9);
    EXPECT_FALSE(rec.frontFace);
    expectVecNear(rec.normal, Vec3(0, 0, -1));
}

TEST(SphereTest, NearRootRejectedByTMinGivesFarRoot) {
    const Sphere s(Vec3(0, 0, -5), 1.0, mat());
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 4.5, INF, rec));
    EXPECT_NEAR(rec.t, 6.0, 1e-9);
    EXPECT_FALSE(rec.frontFace);
}

TEST(SphereTest, NonNormalizedDirectionStillCorrect) {
    const Sphere s(Vec3(0, 0, -5), 1.0, mat());
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -2)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 2.0, 1e-9); // t n'est plus une distance
    expectVecNear(rec.point, Vec3(0, 0, -4));
}

TEST(SphereTest, NormalIsUnit) {
    const Sphere s(Vec3(0, 0, -5), 2.0, mat());
    HitRecord rec;
    ASSERT_TRUE(s.hit(Ray(Vec3(0.3, 0.7, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.normal.length(), 1.0, 1e-12);
}

TEST(SphereTest, InvalidConstruction) {
    EXPECT_THROW(Sphere(Vec3(), 0.0, mat()), std::invalid_argument);
    EXPECT_THROW(Sphere(Vec3(), -1.0, mat()), std::invalid_argument);
    EXPECT_THROW(Sphere(Vec3(), 1.0, nullptr), std::invalid_argument);
}
