// Fichier : Camera.cpp
// Rôle : squelette de construction de la base caméra et des rayons.
// Responsable : Personne A
// Dépendances : Camera.hpp
// TODO : TODO(A): calculer forward/right/trueUp et le plan image avec tan(FOV/2).

#include "camera/Camera.hpp"

namespace rt {

Camera::Camera(const Vec3& position, const Vec3&, const Vec3&,
               double, double) noexcept
    : origin_(position), lowerLeftCorner_(), horizontal_(), vertical_() {}

Ray Camera::getRay(double, double) const {
    // TODO(A): retourner Ray(origin_, lowerLeftCorner_ + u*horizontal_ + v*vertical_ - origin_).
    return Ray(origin_, Vec3{});
}

} // namespace rt
