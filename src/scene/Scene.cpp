// Fichier : Scene.cpp
// Rôle : squelette de gestion des objets et recherche de l'intersection la plus proche.
// Responsable : Personne C
// Dépendances : Scene.hpp
// TODO : TODO(C): parcourir les objets avec closestSoFar et propager le matériau.
// CHAÎNE : SceneParser remplit Scene -> Renderer demande le premier hit ->
// Scene délègue aux primitives -> Material consomme HitRecord.

#include "scene/Scene.hpp"
#include <stdexcept>
#include <utility>

namespace rt {

Scene::Scene()
    // TODO(C): définir les valeurs par défaut documentées de la scène.
    : cameraParams_(), background_(0.0, 0.0, 0.0) {}

bool Scene::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(C): closestSoFar = tMax ; pour chaque objet, appeler hit sur
    // [tMin, closestSoFar], puis mettre à jour closestSoFar et rec. Retourner
    // vrai si au moins une intersection a été retenue.
    return false;
}

void Scene::addObject(std::unique_ptr<Hittable> object) {
    // TODO(C): transférer la propriété de object dans objects_.
    (void)object;
}

void Scene::addMaterial(const std::shared_ptr<Material>& material) {
    // TODO(C): rejeter un matériau nul et signaler un nom dupliqué.
    (void)material;
}

const std::shared_ptr<Material>& Scene::findMaterial(const std::string& name) const {
    // TODO(C): rechercher name et lever une erreur si le matériau est inconnu.
    (void)name;
    static const std::shared_ptr<Material> missing;
    return missing;
}

void Scene::addLight(const PointLight& light) {
    // TODO(C): ajouter la lumière à lights_.
    (void)light;
}

} // namespace rt
