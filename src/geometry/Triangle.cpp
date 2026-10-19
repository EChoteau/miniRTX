// Fichier : Triangle.cpp
// Rôle : squelette OPTIONNEL de Möller-Trumbore.
// Responsable : Personne A (OPTIONNEL)
// Dépendances : Triangle.hpp
// TODO : TODO(A): rejeter determinant proche de zéro et vérifier u, v, u+v.

#include "geometry/Triangle.hpp"
#include <utility>

namespace rt {

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> material) noexcept
    : a_(a), b_(b), c_(c), material_(std::move(material)) {}

bool Triangle::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(A): appliquer Möller-Trumbore, puis orienter la normale.
    return false;
}

} // namespace rt
