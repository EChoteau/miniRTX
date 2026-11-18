#pragma once

// Fichier : Ray.hpp
// Rôle : rayon paramétrique P(t) = origin + t * direction.
// Responsable : Personne A
// Dépendances : Vec3.hpp
// Convention : toutes les directions sont NORMALISÉES à la création (caméra, ombre,
// réflexion). Les intersections restent correctes sans normalisation (a = d·d),
// mais t n'est une distance que si la direction est normalisée. Camera::getRay
// normalise la direction.

#include "core/Vec3.hpp"

namespace rt {

class Ray {
public:
    // EN CLAIR : un rayon est une flèche partant d'une origine vers une direction.
    // ENTRÉES / SORTIES : il reçoit O et D, puis fournit une position P(t).
    // APPELÉ PAR : Camera, les primitives et Renderer.
    // PAS À FAIRE ICI : ne pas décider si une intersection existe.
    // La direction est stockée telle quelle : c'est le CRÉATEUR du rayon (caméra,
    // ombre, réflexion) qui la normalise.
    Ray() noexcept = default;
    Ray(const Vec3& origin, const Vec3& direction) noexcept
        : origin_(origin), direction_(direction) {}

    const Vec3& origin() const noexcept { return origin_; }
    const Vec3& direction() const noexcept { return direction_; }
    // P(t) = origin + t * direction (ne suppose pas D normalisée).
    Vec3 at(double t) const noexcept { return origin_ + t * direction_; }

private:
    Vec3 origin_;
    Vec3 direction_;
};

} // namespace rt
