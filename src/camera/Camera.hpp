#pragma once

// Fichier : Camera.hpp
// Rôle : caméra perspective et génération de rayons primaires.
// Responsable : Personne A
// Dépendances : Ray.hpp, Utils.hpp
// TODO : TODO(A): gérer le cas lookAt == position et un vecteur up parallèle.

#include "core/Ray.hpp"

namespace rt {

class Camera {
public:
    Camera() noexcept = default;
    // FOV vertical en degrés ; aspect = largeur / hauteur.
    Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up,
           double verticalFovDegrees, double aspectRatio) noexcept;
    // u et v sont dans [0,1] ; l'origine image est en bas à gauche.
    Ray getRay(double u, double v) const;

private:
    Vec3 origin_;
    Vec3 lowerLeftCorner_;
    Vec3 horizontal_;
    Vec3 vertical_;
};

} // namespace rt
