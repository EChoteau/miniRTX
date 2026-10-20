// Fichier : test_vec3.cpp
// Rôle : tests de l'algèbre vectorielle.
// Responsable : Personne A
// Dépendances : Vec3.hpp, GoogleTest
// Cas : dot((1,2,3),(4,5,6))=32 ; cross((1,0,0),(0,1,0))=(0,0,1) ;
// length((3,4,0))=5 ; normalized(0,0,0)=(0,0,0) ; reflect=(1,1,0).

#include <gtest/gtest.h>

#include <cmath>

#include "core/Vec3.hpp"

namespace {

void expectVec3Near(const rt::Vec3& actual, const rt::Vec3& expected) {
    EXPECT_DOUBLE_EQ(actual.x(), expected.x());
    EXPECT_DOUBLE_EQ(actual.y(), expected.y());
    EXPECT_DOUBLE_EQ(actual.z(), expected.z());
}

} // namespace

TEST(Vec3Test, SupportsBasicArithmetic) {
    const rt::Vec3 first{1.0, 2.0, 3.0};
    const rt::Vec3 second{4.0, 5.0, 6.0};

    expectVec3Near(first + second, rt::Vec3{5.0, 7.0, 9.0});
    expectVec3Near(second - first, rt::Vec3{3.0, 3.0, 3.0});
    expectVec3Near(-first, rt::Vec3{-1.0, -2.0, -3.0});
    expectVec3Near(first * 2.0, rt::Vec3{2.0, 4.0, 6.0});
    expectVec3Near(2.0 * first, rt::Vec3{2.0, 4.0, 6.0});
    expectVec3Near(second / 2.0, rt::Vec3{2.0, 2.5, 3.0});
    expectVec3Near(first * second, rt::Vec3{4.0, 10.0, 18.0});
    expectVec3Near(second / first, rt::Vec3{4.0, 2.5, 2.0});
}

TEST(Vec3Test, CalculatesProductsAndLengths) {
    const rt::Vec3 first{1.0, 2.0, 3.0};
    const rt::Vec3 second{4.0, 5.0, 6.0};
    const rt::Vec3 lengthVector{3.0, 4.0, 0.0};

    EXPECT_DOUBLE_EQ(rt::dot(first, second), 32.0);
    expectVec3Near(rt::cross(rt::Vec3{1.0, 0.0, 0.0},
                             rt::Vec3{0.0, 1.0, 0.0}),
                   rt::Vec3{0.0, 0.0, 1.0});
    EXPECT_DOUBLE_EQ(lengthVector.lengthSquared(), 25.0);
    EXPECT_DOUBLE_EQ(lengthVector.length(), 5.0);
}

TEST(Vec3Test, NormalizesNonZeroVector) {
    const rt::Vec3 normalized = rt::Vec3{3.0, 4.0, 0.0}.normalized();

    expectVec3Near(normalized, rt::Vec3{0.6, 0.8, 0.0});
    EXPECT_DOUBLE_EQ(normalized.length(), 1.0);
}

TEST(Vec3Test, NormalizingZeroVectorReturnsZeroVector) {
    expectVec3Near(rt::Vec3{}.normalized(), rt::Vec3{});
}

TEST(Vec3Test, DivisionByZeroReturnsZeroComponents) {
    const rt::Vec3 value{1.0, -2.0, 3.0};

    expectVec3Near(value / 0.0, rt::Vec3{});
    expectVec3Near(value / rt::Vec3{0.0, 2.0, 0.0},
                   rt::Vec3{0.0, -1.0, 0.0});
}

TEST(Vec3Test, DivisionByZeroDoesNotProduceNonFiniteValues) {
    const rt::Vec3 result = rt::Vec3{1.0, 2.0, 3.0} / rt::Vec3{0.0, 0.0, 0.0};

    EXPECT_TRUE(std::isfinite(result.x()));
    EXPECT_TRUE(std::isfinite(result.y()));
    EXPECT_TRUE(std::isfinite(result.z()));
}

TEST(Vec3Test, ReflectsVectorAgainstNormal) {
    expectVec3Near(rt::reflect(rt::Vec3{1.0, -1.0, 0.0},
                               rt::Vec3{0.0, 1.0, 0.0}),
                   rt::Vec3{1.0, 1.0, 0.0});
}

TEST(Vec3Test, SupportsIndexingAndMutation) {
    rt::Vec3 value{1.0, 2.0, 3.0};

    EXPECT_DOUBLE_EQ(value[0], 1.0);
    EXPECT_DOUBLE_EQ(value[1], 2.0);
    EXPECT_DOUBLE_EQ(value[2], 3.0);
    value[1] = 8.0;
    expectVec3Near(value, rt::Vec3{1.0, 8.0, 3.0});
}
