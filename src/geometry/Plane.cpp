// Fichier : Plane.cpp
// Rôle : squelette de l'intersection rayon-plan.
// Responsable : Personne A
// Dépendances : Plane.hpp
// TODO : TODO(A): tester denominateur proche de zéro puis calculer t.

#include "geometry/Plane.hpp"
#include <utility>

namespace rt {

Plane::Plane(const Vec3& point, const Vec3& normal, std::shared_ptr<Material> material) noexcept
    : point_(point), normal_(normal.normalized()), material_(std::move(material)) {}

bool Plane::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(A): calculer t = dot(point - origin, normal) / dot(direction, normal).
    return false;
}

} // namespace rt
