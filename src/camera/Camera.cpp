// Fichier : Camera.cpp
// Rôle : squelette de construction de la base caméra et des rayons.
// Responsable : Personne A
// Dépendances : Camera.hpp
// TODO : TODO(A): calculer forward/right/trueUp et le plan image avec tan(FOV/2).
// CHAÎNE : SceneParser fournit CameraParams -> Renderer construit Camera avec
// l'aspect de l'image -> getRay fournit un rayon -> Scene teste les objets.

#include "camera/Camera.hpp"

namespace rt {

Camera::Camera(const Vec3& position, const Vec3&, const Vec3&,
               double, double)
    // TODO(A): valider les paramètres puis calculer la base caméra et la fenêtre.
    : origin_(position), lowerLeftCorner_(), horizontal_(), vertical_() {}

Ray Camera::getRay(double, double) const {
    // TODO(A): calculer le point de fenêtre, normaliser sa direction et retourner
    // Ray(origin_, direction).
    return Ray(origin_, Vec3{});
}

} // namespace rt
