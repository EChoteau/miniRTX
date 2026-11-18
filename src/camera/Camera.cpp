// Fichier : Camera.cpp
// Rôle : construction de la base caméra et génération des rayons primaires.
// Responsable : Personne A
// Dépendances : Camera.hpp
// CHAÎNE : SceneParser fournit CameraParams -> Renderer construit Camera avec
// l'aspect de l'image -> getRay fournit un rayon -> Scene teste les objets.

#include "camera/Camera.hpp"
#include <cmath>
#include <stdexcept>

namespace rt {

Camera::Camera() : Camera(CameraParams{}.position, CameraParams{}.lookAt, CameraParams{}.up,
                          CameraParams{}.verticalFovDegrees, 1.0) {}

Camera::Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up,
               double verticalFovDegrees, double aspectRatio)
    : origin_(position) {
    // 1. Validations.
    if (!(verticalFovDegrees > 0.0 && verticalFovDegrees < 180.0)) {
        throw std::invalid_argument("camera: fov must be in ]0,180[");
    }
    if (!(aspectRatio > 0.0) || !std::isfinite(aspectRatio)) {
        throw std::invalid_argument("camera: aspect ratio must be > 0");
    }
    const Vec3 forward = (lookAt - position).normalized();
    if (forward.lengthSquared() == 0.0) {
        throw std::invalid_argument("camera: lookAt must differ from position");
    }

    // 2. Base orthonormée. Si up est nul ou parallèle à la visée, on prend
    //    (0,0,1) ; si c'est encore parallèle (visée selon Z), (0,1,0).
    Vec3 right = cross(forward, up).normalized();
    if (right.lengthSquared() == 0.0) {
        right = cross(forward, Vec3{0, 0, 1}).normalized();
    }
    if (right.lengthSquared() == 0.0) {
        right = cross(forward, Vec3{0, 1, 0}).normalized();
    }
    const Vec3 trueUp = cross(right, forward);

    // 3. Fenêtre à distance 1 devant l'œil.
    const double halfH = std::tan(degreesToRadians(verticalFovDegrees) / 2.0);
    const double halfW = aspectRatio * halfH;
    horizontal_ = 2.0 * halfW * right;
    vertical_ = 2.0 * halfH * trueUp;
    lowerLeftCorner_ = origin_ + forward - horizontal_ / 2.0 - vertical_ / 2.0;
}

Ray Camera::getRay(double u, double v) const {
    const Vec3 target = lowerLeftCorner_ + u * horizontal_ + v * vertical_;
    return Ray(origin_, (target - origin_).normalized());
}

} // namespace rt
