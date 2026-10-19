#pragma once

// Fichier : Ray.hpp
// Rôle : rayon paramétrique P(t) = origin + t * direction.
// Responsable : Personne A
// Dépendances : Vec3.hpp
// TODO : TODO(A): documenter les conventions de direction normalisée côté caméra.

#include "core/Vec3.hpp"

namespace rt {

class Ray {
public:
    // Un rayon possède une origine et une direction ; t négatif est ignoré par les intersections.
    Ray() noexcept = default;
    Ray(const Vec3& origin, const Vec3& direction) noexcept
        : origin_(origin), direction_(direction) {}

    const Vec3& origin() const noexcept { return origin_; }
    const Vec3& direction() const noexcept { return direction_; }
    // Évalue P(t), sans supposer que direction est normalisée.
    Vec3 at(double t) const noexcept { return origin_ + t * direction_; }

private:
    Vec3 origin_;
    Vec3 direction_;
};

} // namespace rt
