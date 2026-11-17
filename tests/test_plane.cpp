// Fichier : test_plane.cpp
// Rôle : tests d'intersection rayon-plan.
// Responsable : Personne A
// Dépendances : Plane.hpp, GoogleTest

#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include "geometry/Plane.hpp"

using rt::HitRecord;
using rt::Plane;
using rt::Ray;
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

TEST(PlaneTest, HitFromAbove) {
    auto m = mat();
    const Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), m);
    HitRecord rec;
    ASSERT_TRUE(p.hit(Ray(Vec3(0, 2, 0), Vec3(0, -1, 0)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 2.0, 1e-9);
    expectVecNear(rec.point, Vec3(0, 0, 0));
    expectVecNear(rec.normal, Vec3(0, 1, 0));
    EXPECT_TRUE(rec.frontFace);
    EXPECT_EQ(rec.material, m.get());
}

TEST(PlaneTest, ParallelRayMisses) {
    const Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), mat());
    HitRecord rec;
    EXPECT_FALSE(p.hit(Ray(Vec3(0, 2, 0), Vec3(1, 0, 0)), 0.0, INF, rec));
}

TEST(PlaneTest, RayGoingAwayMisses) {
    const Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), mat());
    HitRecord rec;
    EXPECT_FALSE(p.hit(Ray(Vec3(0, 2, 0), Vec3(0, 1, 0)), 0.0, INF, rec));
}

TEST(PlaneTest, HitFromBelowFlipsNormal) {
    const Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), mat());
    HitRecord rec;
    ASSERT_TRUE(p.hit(Ray(Vec3(0, -1, 0), Vec3(0, 1, 0)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 1.0, 1e-9);
    EXPECT_FALSE(rec.frontFace);
    expectVecNear(rec.normal, Vec3(0, -1, 0));
}

TEST(PlaneTest, ObliqueRayAndTMax) {
    const Plane p(Vec3(0, 0, 0), Vec3(0, 1, 0), mat());
    const Ray r(Vec3(0, 1, 0), Vec3(1, -1, 0).normalized());
    HitRecord rec;
    ASSERT_TRUE(p.hit(r, 0.0, INF, rec));
    EXPECT_NEAR(rec.t, std::sqrt(2.0), 1e-9);
    expectVecNear(rec.point, Vec3(1, 0, 0));
    EXPECT_FALSE(p.hit(r, 0.0, 1.0, rec));
}

TEST(PlaneTest, NonUnitNormalIsNormalized) {
    const Plane p(Vec3(0, 3, 0), Vec3(0, 5, 0), mat());
    HitRecord rec;
    ASSERT_TRUE(p.hit(Ray(Vec3(0, 5, 0), Vec3(0, -1, 0)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 2.0, 1e-9);
    expectVecNear(rec.normal, Vec3(0, 1, 0));
}

TEST(PlaneTest, InvalidConstruction) {
    EXPECT_THROW(Plane(Vec3(), Vec3(0, 0, 0), mat()), std::invalid_argument);
    EXPECT_THROW(Plane(Vec3(), Vec3(0, 1, 0), nullptr), std::invalid_argument);
}
