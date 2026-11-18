// Fichier : test_camera.cpp
// Rôle : tests de génération des rayons caméra.
// Responsable : Personne A
// Dépendances : Camera.hpp, GoogleTest

#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include "camera/Camera.hpp"

using rt::Camera;
using rt::Vec3;

namespace {
void expectVecNear(const Vec3& a, const Vec3& b, double tol = 1e-9) {
    EXPECT_NEAR(a.x(), b.x(), tol);
    EXPECT_NEAR(a.y(), b.y(), tol);
    EXPECT_NEAR(a.z(), b.z(), tol);
}
bool hasNaN(const Vec3& v) {
    return std::isnan(v.x()) || std::isnan(v.y()) || std::isnan(v.z());
}
Camera fov90(double aspect) {
    return Camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90.0, aspect);
}
} // namespace

TEST(CameraTest, CenterLooksForward) {
    const rt::Ray r = fov90(1.0).getRay(0.5, 0.5);
    expectVecNear(r.origin(), Vec3(0, 0, 0));
    expectVecNear(r.direction(), Vec3(0, 0, -1));
}

TEST(CameraTest, Corners) {
    const Camera cam = fov90(1.0);
    expectVecNear(cam.getRay(0, 0).direction(), Vec3(-1, -1, -1).normalized());
    expectVecNear(cam.getRay(1, 1).direction(), Vec3(1, 1, -1).normalized());
    expectVecNear(cam.getRay(1, 0).direction(), Vec3(1, -1, -1).normalized());
    expectVecNear(cam.getRay(0, 1).direction(), Vec3(-1, 1, -1).normalized());
}

TEST(CameraTest, AspectRatio2) {
    const Camera cam = fov90(2.0);
    expectVecNear(cam.getRay(1, 0.5).direction(), Vec3(2, 0, -1).normalized());
    expectVecNear(cam.getRay(0.5, 1).direction(), Vec3(0, 1, -1).normalized());
}

TEST(CameraTest, DirectionsAreNormalized) {
    const Camera cam = fov90(16.0 / 9.0);
    for (double u : {0.0, 0.25, 0.5, 0.9, 1.0}) {
        for (double v : {0.0, 0.3, 0.5, 1.0}) {
            EXPECT_NEAR(cam.getRay(u, v).direction().length(), 1.0, 1e-12);
        }
    }
}

TEST(CameraTest, Fov45WindowHeight) {
    // tan(22.5°) ≈ 0.4142 : le haut de la fenêtre est à y ≈ 0.4142 pour z = -1.
    const Camera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 45.0, 1.0);
    const Vec3 d = cam.getRay(0.5, 1.0).direction();
    EXPECT_NEAR(d.y() / -d.z(), std::tan(rt::PI / 8.0), 1e-9);
}

TEST(CameraTest, OffsetPositionLooksAtTarget) {
    const Camera cam(Vec3(0, 1.8, 6), Vec3(0, 1, 0), Vec3(0, 1, 0), 45.0, 16.0 / 9.0);
    const rt::Ray r = cam.getRay(0.5, 0.5);
    expectVecNear(r.origin(), Vec3(0, 1.8, 6));
    expectVecNear(r.direction(), (Vec3(0, 1, 0) - Vec3(0, 1.8, 6)).normalized());
}

TEST(CameraTest, LookAtEqualsPositionThrows) {
    EXPECT_THROW(Camera(Vec3(1, 2, 3), Vec3(1, 2, 3), Vec3(0, 1, 0), 60.0, 1.0),
                 std::invalid_argument);
}

TEST(CameraTest, InvalidFovOrAspectThrows) {
    EXPECT_THROW(Camera(Vec3(), Vec3(0, 0, -1), Vec3(0, 1, 0), 0.0, 1.0),
                 std::invalid_argument);
    EXPECT_THROW(Camera(Vec3(), Vec3(0, 0, -1), Vec3(0, 1, 0), 180.0, 1.0),
                 std::invalid_argument);
    EXPECT_THROW(Camera(Vec3(), Vec3(0, 0, -1), Vec3(0, 1, 0), 60.0, 0.0),
                 std::invalid_argument);
}

TEST(CameraTest, UpParallelToViewHasNoNaN) {
    // Regarde vers le bas avec up = (0,1,0) : parallèle -> repli sur (0,0,1).
    const Camera cam(Vec3(0, 5, 0), Vec3(0, 0, 0), Vec3(0, 1, 0), 60.0, 1.0);
    for (double u : {0.0, 0.5, 1.0}) {
        for (double v : {0.0, 0.5, 1.0}) {
            const Vec3 d = cam.getRay(u, v).direction();
            EXPECT_FALSE(hasNaN(d));
            EXPECT_NEAR(d.length(), 1.0, 1e-12);
        }
    }
    expectVecNear(cam.getRay(0.5, 0.5).direction(), Vec3(0, -1, 0));
}

TEST(CameraTest, ZeroUpHasNoNaN) {
    const Camera cam(Vec3(), Vec3(0, 0, -1), Vec3(0, 0, 0), 60.0, 1.0);
    EXPECT_FALSE(hasNaN(cam.getRay(0.2, 0.8).direction()));
    expectVecNear(cam.getRay(0.5, 0.5).direction(), Vec3(0, 0, -1));
}

TEST(CameraTest, DefaultCameraMatchesCameraParams) {
    const Camera cam;
    const rt::CameraParams p;
    const rt::Ray r = cam.getRay(0.5, 0.5);
    expectVecNear(r.origin(), p.position);
    expectVecNear(r.direction(), (p.lookAt - p.position).normalized());
}
