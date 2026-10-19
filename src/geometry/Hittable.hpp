#pragma once

// Fichier : Hittable.hpp
// Rôle : interface abstraite commune à toutes les primitives intersectables.
// Responsable : Personne A
// Dépendances : HitRecord.hpp, Ray.hpp
// TODO : TODO(A): envisager une boîte englobante pour une future BVH.

#include "geometry/HitRecord.hpp"

namespace rt {

class Hittable {
public:
    virtual ~Hittable() = default;
    // Cherche une intersection dans l'intervalle fermé conceptuellement [tMin, tMax].
    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};

} // namespace rt
