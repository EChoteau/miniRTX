// Fichier : test_vec3.cpp
// Rôle : tests de l'algèbre vectorielle, de Utils, Color et Ray.
// Responsable : Personne A
// Dépendances : Vec3.hpp, Color.hpp, Ray.hpp, Utils.hpp, GoogleTest

#include <gtest/gtest.h>
#include <cmath>
#include "core/Color.hpp"
#include "core/Ray.hpp"
#include "core/Utils.hpp"
#include "core/Vec3.hpp"

using rt::Vec3;

namespace {
void expectVecNear(const Vec3& a, const Vec3& b, double tol = 1e-9) {
    EXPECT_NEAR(a.x(), b.x(), tol);
    EXPECT_NEAR(a.y(), b.y(), tol);
    EXPECT_NEAR(a.z(), b.z(), tol);
}
} // namespace

TEST(Vec3Test, ConstructionAndAccessors) {
    Vec3 v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v.x(), 1.0);
    EXPECT_DOUBLE_EQ(v.y(), 2.0);
    EXPECT_DOUBLE_EQ(v.z(), 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    v[1] = 5.0;
    v.z() = 7.0;
    expectVecNear(v, Vec3(1, 5, 7));
    expectVecNear(Vec3{}, Vec3(0, 0, 0));
}

TEST(Vec3Test, Arithmetic) {
    const Vec3 a(1, 2, 3);
    const Vec3 b(4, 5, 6);
    expectVecNear(a + b, Vec3(5, 7, 9));
    expectVecNear(b - a, Vec3(3, 3, 3));
    expectVecNear(-a, Vec3(-1, -2, -3));
    expectVecNear(a * 2.0, Vec3(2, 4, 6));
    expectVecNear(2.0 * a, Vec3(2, 4, 6));
    expectVecNear(b / 2.0, Vec3(2, 2.5, 3));
    expectVecNear(a * b, Vec3(4, 10, 18));
    expectVecNear(b / a, Vec3(4, 2.5, 2));

    Vec3 c = a;
    c += b;
    expectVecNear(c, Vec3(5, 7, 9));
    c -= b;
    expectVecNear(c, a);
    c *= 3.0;
    expectVecNear(c, Vec3(3, 6, 9));
    c /= 3.0;
    expectVecNear(c, a);
}

TEST(Vec3Test, Dot) {
    EXPECT_DOUBLE_EQ(rt::dot(Vec3(1, 2, 3), Vec3(4, 5, 6)), 32.0);
    EXPECT_DOUBLE_EQ(rt::dot(Vec3(1, 0, 0), Vec3(0, 1, 0)), 0.0);
    EXPECT_DOUBLE_EQ(rt::dot(Vec3(1, 0, 0), Vec3(-1, 0, 0)), -1.0);
}

TEST(Vec3Test, Cross) {
    expectVecNear(rt::cross(Vec3(1, 0, 0), Vec3(0, 1, 0)), Vec3(0, 0, 1));
    expectVecNear(rt::cross(Vec3(0, 1, 0), Vec3(0, 0, 1)), Vec3(1, 0, 0));
    expectVecNear(rt::cross(Vec3(0, 0, 1), Vec3(1, 0, 0)), Vec3(0, 1, 0));
    expectVecNear(rt::cross(Vec3(0, 1, 0), Vec3(1, 0, 0)), Vec3(0, 0, -1));
    // Le résultat est perpendiculaire aux deux opérandes.
    const Vec3 a(1, 2, 3), b(4, 5, 6);
    const Vec3 c = rt::cross(a, b);
    EXPECT_NEAR(rt::dot(c, a), 0.0, 1e-12);
    EXPECT_NEAR(rt::dot(c, b), 0.0, 1e-12);
    expectVecNear(rt::cross(a, a), Vec3(0, 0, 0));
}

TEST(Vec3Test, Length) {
    EXPECT_DOUBLE_EQ(Vec3(3, 4, 0).length(), 5.0);
    EXPECT_DOUBLE_EQ(Vec3(3, 4, 0).lengthSquared(), 25.0);
    EXPECT_DOUBLE_EQ(Vec3().length(), 0.0);
}

TEST(Vec3Test, Normalized) {
    expectVecNear(Vec3(3, 4, 0).normalized(), Vec3(0.6, 0.8, 0));
    EXPECT_NEAR(Vec3(1, 2, 3).normalized().length(), 1.0, 1e-12);
}

TEST(Vec3Test, NormalizedZeroVectorIsZeroWithoutNaN) {
    const Vec3 n = Vec3(0, 0, 0).normalized();
    EXPECT_FALSE(std::isnan(n.x()) || std::isnan(n.y()) || std::isnan(n.z()));
    expectVecNear(n, Vec3(0, 0, 0));
    // En dessous d'EPSILON : considéré comme nul aussi.
    expectVecNear(Vec3(1e-9, 0, 0).normalized(), Vec3(0, 0, 0));
}

TEST(Vec3Test, Reflect) {
    expectVecNear(rt::reflect(Vec3(1, -1, 0), Vec3(0, 1, 0)), Vec3(1, 1, 0));
    // Incidence normale : le rayon repart en sens inverse.
    expectVecNear(rt::reflect(Vec3(0, 0, -1), Vec3(0, 0, 1)), Vec3(0, 0, 1));
    // Vecteur rasant : inchangé.
    expectVecNear(rt::reflect(Vec3(1, 0, 0), Vec3(0, 1, 0)), Vec3(1, 0, 0));
}

TEST(UtilsTest, Clamp) {
    EXPECT_DOUBLE_EQ(rt::clamp(0.5, 0.0, 1.0), 0.5);
    EXPECT_DOUBLE_EQ(rt::clamp(-2.0, 0.0, 1.0), 0.0);
    EXPECT_DOUBLE_EQ(rt::clamp(3.0, 0.0, 1.0), 1.0);
}

TEST(UtilsTest, DegreesToRadians) {
    EXPECT_DOUBLE_EQ(rt::degreesToRadians(180.0), rt::PI);
    EXPECT_DOUBLE_EQ(rt::degreesToRadians(90.0), rt::PI / 2.0);
    EXPECT_DOUBLE_EQ(rt::degreesToRadians(0.0), 0.0);
}

TEST(ColorTest, ClampColor) {
    expectVecNear(rt::clampColor(rt::Color(-0.5, 0.25, 1.5)), rt::Color(0.0, 0.25, 1.0));
}

TEST(ColorTest, ToRgb8) {
    const auto black = rt::toRgb8(rt::Color(0, 0, 0));
    const auto white = rt::toRgb8(rt::Color(1, 1, 1));
    const auto mid = rt::toRgb8(rt::Color(0.5, 0.2, 1.0));
    EXPECT_EQ(black[0], 0);
    EXPECT_EQ(white[0], 255);
    EXPECT_EQ(white[2], 255);
    EXPECT_EQ(mid[0], 128); // 127.5 arrondi au plus proche
    EXPECT_EQ(mid[1], 51);
    EXPECT_EQ(mid[2], 255);
    // Hors bornes : saturé, pas de dépassement d'entier.
    const auto sat = rt::toRgb8(rt::Color(-1.0, 2.0, 0.0));
    EXPECT_EQ(sat[0], 0);
    EXPECT_EQ(sat[1], 255);
}

TEST(RayTest, AccessorsAndAt) {
    const rt::Ray r(Vec3(1, 2, 3), Vec3(0, 0, -1));
    expectVecNear(r.origin(), Vec3(1, 2, 3));
    expectVecNear(r.direction(), Vec3(0, 0, -1));
    expectVecNear(r.at(0.0), Vec3(1, 2, 3));
    expectVecNear(r.at(4.0), Vec3(1, 2, -1));
    // at() ne suppose pas une direction normalisée.
    const rt::Ray r2(Vec3(0, 0, 0), Vec3(2, 0, 0));
    expectVecNear(r2.at(1.5), Vec3(3, 0, 0));
}
