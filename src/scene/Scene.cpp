// Fichier : Scene.cpp
// Rôle : Gestion des objets et recherche de l'intersection la plus proche.
// Responsable : Personne C
// Dépendances : Scene.hpp
// CHAÎNE : SceneParser remplit Scene -> Renderer demande le premier hit ->
// Scene délègue aux primitives -> Material consomme HitRecord.

#include "scene/Scene.hpp"
#include <stdexcept>
#include <utility>

namespace rt {

/// Initialise la scène avec une caméra par défaut et un fond noir.
Scene::Scene() : cameraParams_(), background_(0.0, 0.0, 0.0) {}

/// Retourne l'intersection la plus proche avec les objets de la scène.
bool Scene::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    HitRecord temp;
    bool hasHitAnything = false;
    double closestSoFar = tMax;
    for (const auto& object : objects_) {
        if (object->hit(r, tMin, closestSoFar, temp)) {
            hasHitAnything = true;
            closestSoFar = temp.t;
            rec = temp;
        }
    }
    return hasHitAnything;
}

/// Ajoute un objet et en transfère la propriété à la scène.
void Scene::addObject(std::unique_ptr<Hittable> object) {
    if (!object) throw std::invalid_argument("null object");
    objects_.push_back(std::move(object)); //move pour le unique_ptr
}

/// Ajoute un matériau dont le nom doit être unique.
void Scene::addMaterial(const std::shared_ptr<Material>& material) {
    if (!material) throw std::invalid_argument("null material");
    if (!materials_.emplace(material->name, material).second)
        throw std::invalid_argument("duplicate material '" + material->name + "'");
}

/// Recherche un matériau par son nom.
const std::shared_ptr<Material>& Scene::findMaterial(const std::string& name) const {
    auto it = materials_.find(name);
    if (it == materials_.end())
        throw std::invalid_argument("unknown material '" + name + "'");
    return it->second;
}

/// Ajoute une lumière à la scène.
void Scene::addLight(const PointLight& light){
    lights_.push_back(light); 
}


} // namespace rt
