// Fichier : Plane.cpp
// Rôle : squelette de l'intersection rayon-plan.
// Responsable : Personne A
// Dépendances : Plane.hpp
// TODO : TODO(A): tester denominateur proche de zéro puis calculer t.
// CHAÎNE : le rayon arrive de Camera ou d'une lumière -> le plan teste le
// dénominateur -> HitRecord sert ensuite au shading.

#include "geometry/Plane.hpp"
#include <utility>

namespace rt {

Plane::Plane(const Vec3& point, const Vec3& normal, std::shared_ptr<Material> material)
    // TODO(A): valider la normale et le matériau puis normaliser la normale.
    : point_(point), normal_(normal.normalized()), material_(std::move(material)) {}

bool Plane::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(A): calculer t = dot(point - origin, normal) / dot(direction, normal).
    return false;
}

} // namespace rt
