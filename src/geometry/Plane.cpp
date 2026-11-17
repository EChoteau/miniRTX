// Fichier : Plane.cpp
// Rôle : intersection rayon-plan.
// Responsable : Personne A
// Dépendances : Plane.hpp
// CHAÎNE : le rayon arrive de Camera ou d'une lumière -> le plan teste le
// dénominateur -> HitRecord sert ensuite au shading.

#include "geometry/Plane.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace rt {

namespace {
Vec3 checkedNormal(const Vec3& n) {
    if (!(n.length() > EPSILON)) {
        throw std::invalid_argument("plane: normal must be non-zero");
    }
    return n.normalized();
}
} // namespace

Plane::Plane(const Vec3& point, const Vec3& normal, std::shared_ptr<Material> material)
    : point_(point), normal_(checkedNormal(normal)), material_(std::move(material)) {
    if (!material_) {
        throw std::invalid_argument("plane: material is null");
    }
}

bool Plane::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    // 1. Rayon parallèle au plan (ou direction nulle) : pas d'intersection.
    const double denom = dot(normal_, r.direction());
    if (std::abs(denom) < EPSILON) {
        return false;
    }
    // 2. t = ((point - O)·N) / (N·D), à garder dans ]tMin, tMax[.
    const double t = dot(point_ - r.origin(), normal_) / denom;
    if (t <= tMin || t >= tMax) {
        return false;
    }
    // 3. Remplir le HitRecord (normale orientée contre le rayon).
    rec.t = t;
    rec.point = r.at(t);
    rec.setFaceNormal(r, normal_);
    rec.material = material_.get();
    return true;
}

} // namespace rt
