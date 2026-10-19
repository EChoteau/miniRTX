// Fichier : Scene.cpp
// Rôle : squelette de gestion des objets et recherche de l'intersection la plus proche.
// Responsable : Personne C
// Dépendances : Scene.hpp
// TODO : TODO(C): parcourir les objets avec closestSoFar et propager le matériau.

#include "scene/Scene.hpp"
#include <stdexcept>
#include <utility>

namespace rt {

Scene::Scene() : camera_(), background_(0.0, 0.0, 0.0) {}

bool Scene::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(C): appeler Hittable::hit sur chaque objet avec tMax mis à jour.
    return false;
}

void Scene::addObject(std::unique_ptr<Hittable> object) {
    objects_.push_back(std::move(object));
}

void Scene::addMaterial(const std::shared_ptr<Material>& material) {
    if (!material) {
        throw std::invalid_argument("cannot add a null material");
    }
    materials_[material->name] = material;
}

const std::shared_ptr<Material>& Scene::findMaterial(const std::string& name) const {
    const auto iterator = materials_.find(name);
    if (iterator == materials_.end()) {
        throw std::runtime_error("unknown material: " + name);
    }
    return iterator->second;
}

void Scene::addLight(const PointLight& light) {
    lights_.push_back(light);
}

} // namespace rt
