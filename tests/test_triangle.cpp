// Fichier : test_triangle.cpp
// Rôle : tests d'intersection rayon-triangle (Möller-Trumbore).
// Responsable : Personne A
// Dépendances : Triangle.hpp, GoogleTest

#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include "geometry/Triangle.hpp"

using rt::HitRecord;
using rt::Ray;
using rt::Triangle;
using rt::Vec3;

namespace {
std::shared_ptr<rt::Material> mat() { return std::make_shared<rt::Material>(); }
constexpr double INF = rt::INFINITY_VALUE;

// Triangle dans le plan z = -2, normale extérieure (0,0,1) (vers la caméra).
Triangle makeTri(std::shared_ptr<rt::Material> m = mat()) {
    return Triangle(Vec3(-1, -1, -2), Vec3(1, -1, -2), Vec3(0, 1, -2), std::move(m));
}
} // namespace

TEST(TriangleTest, HitCenter) {
    auto m = mat();
    const Triangle tri = makeTri(m);
    HitRecord rec;
    ASSERT_TRUE(tri.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 2.0, 1e-9);
    EXPECT_NEAR(rec.point.z(), -2.0, 1e-9);
    EXPECT_NEAR(rec.normal.z(), 1.0, 1e-9);
    EXPECT_TRUE(rec.frontFace);
    EXPECT_EQ(rec.material, m.get());
}

TEST(TriangleTest, MissOutside) {
    const Triangle tri = makeTri();
    HitRecord rec;
    EXPECT_FALSE(tri.hit(Ray(Vec3(2, 0, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
    EXPECT_FALSE(tri.hit(Ray(Vec3(0, 1.5, 0), Vec3(0, 0, -1)), 0.0, INF, rec));
}

TEST(TriangleTest, ParallelMisses) {
    const Triangle tri = makeTri();
    HitRecord rec;
    EXPECT_FALSE(tri.hit(Ray(Vec3(0, 0, -2), Vec3(1, 0, 0)), 0.0, INF, rec));
}

TEST(TriangleTest, BackFace) {
    const Triangle tri = makeTri();
    HitRecord rec;
    ASSERT_TRUE(tri.hit(Ray(Vec3(0, 0, -4), Vec3(0, 0, 1)), 0.0, INF, rec));
    EXPECT_NEAR(rec.t, 2.0, 1e-9);
    EXPECT_FALSE(rec.frontFace);
    EXPECT_NEAR(rec.normal.z(), -1.0, 1e-9);
}

TEST(TriangleTest, TRange) {
    const Triangle tri = makeTri();
    HitRecord rec;
    EXPECT_FALSE(tri.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, -1)), 0.0, 1.5, rec));
    EXPECT_FALSE(tri.hit(Ray(Vec3(0, 0, 0), Vec3(0, 0, 1)), 0.0, INF, rec));
}

TEST(TriangleTest, InvalidConstruction) {
    EXPECT_THROW(Triangle(Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(2, 2, 2), mat()),
                 std::invalid_argument);
    EXPECT_THROW(Triangle(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(1, 0, 0), mat()),
                 std::invalid_argument);
    EXPECT_THROW(Triangle(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0), nullptr),
                 std::invalid_argument);
}
