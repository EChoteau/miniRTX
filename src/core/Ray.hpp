#pragma once

// Fichier : Ray.hpp
// Rôle : rayon paramétrique P(t) = origin + t * direction.
// Responsable : Personne A
// Dépendances : Vec3.hpp
// TODO(A): toutes les directions sont NORMALISÉES à la création (caméra, ombre,
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
    // TODO(A): construire un rayon avec une direction normalisée.
    Ray() noexcept = default;
    Ray(const Vec3& origin, const Vec3& direction) noexcept
        : origin_(origin), direction_(direction) {}

    // TODO(A): retourner l'origine stockée.
    const Vec3& origin() const noexcept {
        // TODO(A): retourner origin_.
        static const Vec3 placeholder;
        return placeholder;
    }
    // TODO(A): retourner direction_.
    const Vec3& direction() const noexcept {
        static const Vec3 placeholder;
        return placeholder;
    }
    // TODO(A): calculer P(t)=origin+t*direction sans supposer D normalisée.
    Vec3 at(double) const noexcept { return Vec3{}; }

private:
    Vec3 origin_;
    Vec3 direction_;
};

} // namespace rt
