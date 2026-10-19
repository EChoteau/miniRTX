// Fichier : Material.cpp
// Rôle : squelette du modèle de Phong et des ombres.
// Responsable : Personne B
// Dépendances : Material.hpp, Scene.hpp, Light.hpp
// TODO : TODO(B): additionner ambiant, diffus max(dot(n,l),0) et spéculaire.
// CHAÎNE : HitRecord arrive de Scene -> Material évalue Phong et les ombres ->
// Renderer ajoute éventuellement le rayon réfléchi.

#include "material/Material.hpp"
#include "scene/Scene.hpp"
#include "lighting/Light.hpp"

namespace rt {

Color computeLocalShading(const HitRecord&, const Ray&, const Scene&) {
    // TODO(B): lancer un rayon d'ombre avec RAY_T_MIN avant chaque lumière.
    return Color{};
}

bool isInShadow(const Vec3&, const PointLight&, const Scene&) {
    // TODO(B): bornes tMin=RAY_T_MIN et tMax=distance lumière - RAY_T_MIN.
    return false;
}

} // namespace rt
