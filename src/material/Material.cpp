// Fichier : Material.cpp
// Rôle : squelette du modèle de Phong et des ombres.
// Responsable : Personne B
// Dépendances : Material.hpp, Scene.hpp, Light.hpp
// TODO : TODO(B): additionner ambiant, diffus max(dot(n,l),0) et spéculaire.

#include "material/Material.hpp"
#include "scene/Scene.hpp"
#include "lighting/Light.hpp"

namespace rt {

Color computeLocalShading(const HitRecord&, const Ray&, const Scene&) {
    // TODO(B): lancer un rayon d'ombre avec EPSILON avant chaque lumière.
    return Color{};
}

bool isInShadow(const Vec3&, const PointLight&, const Scene&) {
    // TODO(B): bornes tMin=EPSILON et tMax=distance lumière - EPSILON.
    return false;
}

} // namespace rt
