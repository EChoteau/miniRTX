#pragma once

// Fichier : Camera.hpp
// Rôle : caméra perspective et génération de rayons primaires.
// Responsable : Personne A
// Dépendances : Ray.hpp, Utils.hpp
// TODO(A): lancer std::invalid_argument si lookAt == position ; si up est
// parallèle à la visée, utiliser (0,0,1) à la place.

#include "core/Ray.hpp"

namespace rt {

struct CameraParams {
    Vec3 position{0, 1, 5};
    Vec3 lookAt{0, 0, 0};
    Vec3 up{0, 1, 0};
    double verticalFovDegrees{45.0};
};

class Camera {
public:
    // EN CLAIR : Camera transforme une coordonnée de pixel en rayon 3D, comme
    // une fenêtre placée devant l'œil.
    // ENTRÉES / SORTIES : elle reçoit pose, FOV et aspect, puis renvoie un Ray.
    // APPELÉ PAR : Renderer::render.
    // PAS À FAIRE ICI : ne pas intersecter la scène ni choisir une couleur.
    // TODO(A): construire la base orthonormée et traiter les cas dégénérés.
    // TODO(A): fournir une caméra par défaut cohérente avec CameraParams.
    Camera() noexcept = default;
    // FOV vertical en degrés ; aspect = largeur / hauteur.
    Camera(const Vec3& position, const Vec3& lookAt, const Vec3& up,
           double verticalFovDegrees, double aspectRatio);
    // u et v sont dans [0,1] ; l'origine image est en bas à gauche.
    Ray getRay(double u, double v) const;

private:
    Vec3 origin_;
    Vec3 lowerLeftCorner_;
    Vec3 horizontal_;
    Vec3 vertical_;
};

} // namespace rt
