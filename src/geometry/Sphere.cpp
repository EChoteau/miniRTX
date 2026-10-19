// Fichier : Sphere.cpp
// Rôle : squelette de l'intersection rayon-sphère.
// Responsable : Personne A
// Dépendances : Sphere.hpp
// TODO : TODO(A): résoudre le discriminant et remplir HitRecord.

#include "geometry/Sphere.hpp"
#include <utility>

namespace rt {

Sphere::Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material) noexcept
    : center_(center), radius_(radius), material_(std::move(material)) {}

bool Sphere::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(A): implémenter la formule quadratique et setFaceNormal.
    return false;
}

} // namespace rt
