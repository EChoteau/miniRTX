// Fichier : Sphere.cpp
// Rôle : intersection rayon-sphère.
// Responsable : Personne A
// Dépendances : Sphere.hpp
// CHAÎNE : Renderer fournit un Ray -> Scene appelle hit -> Sphere calcule ses
// racines -> HitRecord repart vers le shading.

#include "geometry/Sphere.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace rt {

Sphere::Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material)
    : center_(center), radius_(radius), material_(std::move(material)) {
    if (!(radius_ > EPSILON)) { // rejette aussi NaN
        throw std::invalid_argument("sphere: radius must be > 0");
    }
    if (!material_) {
        throw std::invalid_argument("sphere: material is null");
    }
}

bool Sphere::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    // 1. Coefficients de |O + tD - C|² = r² : a t² + 2 half_b t + c = 0.
    const Vec3 oc = r.origin() - center_;
    const double a = r.direction().lengthSquared();
    if (a <= EPSILON * EPSILON) {
        return false; // direction nulle : pas de rayon
    }
    const double halfB = dot(oc, r.direction());
    const double c = oc.lengthSquared() - radius_ * radius_;

    // 2. Discriminant : < 0 -> le rayon rate la sphère (= 0 -> tangent, c'est un hit).
    const double disc = halfB * halfB - a * c;
    if (disc < 0.0) {
        return false;
    }
    const double sqrtDisc = std::sqrt(disc);

    // 3. Racine proche d'abord, puis la lointaine si la proche est hors ]tMin, tMax[.
    double t = (-halfB - sqrtDisc) / a;
    if (t <= tMin || t >= tMax) {
        t = (-halfB + sqrtDisc) / a;
        if (t <= tMin || t >= tMax) {
            return false;
        }
    }

    // 4. Remplir le HitRecord.
    rec.t = t;
    rec.point = r.at(t);
    const Vec3 outwardNormal = (rec.point - center_) / radius_;
    rec.setFaceNormal(r, outwardNormal);
    rec.material = material_.get();
    return true;
}

} // namespace rt
