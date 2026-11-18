// Fichier : Triangle.cpp
// Rôle : intersection rayon-triangle par Möller-Trumbore.
// Responsable : Personne A (OPTIONNEL)
// Dépendances : Triangle.hpp
// CHAÎNE : Scene délègue au triangle -> Möller-Trumbore teste les barycentriques
// -> le record complet est transmis à Renderer et Material.

#include "geometry/Triangle.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace rt {

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
                   std::shared_ptr<Material> material)
    : a_(a), b_(b), c_(c), material_(std::move(material)) {
    // Triangle dégénéré : sommets alignés ou confondus -> aire nulle.
    if (!(cross(b_ - a_, c_ - a_).length() > EPSILON)) {
        throw std::invalid_argument("triangle: degenerate (zero area)");
    }
    if (!material_) {
        throw std::invalid_argument("triangle: material is null");
    }
}

bool Triangle::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    // Möller-Trumbore : O + tD = A + u e1 + v e2.
    const Vec3 e1 = b_ - a_;
    const Vec3 e2 = c_ - a_;
    const Vec3 p = cross(r.direction(), e2);
    const double det = dot(e1, p);
    if (std::abs(det) < EPSILON) {
        return false; // rayon parallèle au plan du triangle
    }
    const double invDet = 1.0 / det;

    const Vec3 s = r.origin() - a_;
    const double u = dot(s, p) * invDet;
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    const Vec3 q = cross(s, e1);
    const double v = dot(r.direction(), q) * invDet;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    const double t = dot(e2, q) * invDet;
    if (t <= tMin || t >= tMax) {
        return false;
    }

    rec.t = t;
    rec.point = r.at(t);
    // Normale "extérieure" = sens trigonométrique A -> B -> C.
    rec.setFaceNormal(r, cross(e1, e2).normalized());
    rec.material = material_.get();
    return true;
}

} // namespace rt
