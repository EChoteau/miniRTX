#pragma once

// Fichier : Scene.hpp
// Rôle : propriété des objets, matériaux, lumières, caméra et fond.
// Responsable : Personne C
// Dépendances : Camera.hpp, Hittable.hpp, Light.hpp, Material.hpp
// TODO : TODO(C): parcourir les objets et retenir l'intersection t minimale.

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "camera/Camera.hpp"
#include "geometry/Hittable.hpp"
#include "lighting/Light.hpp"
#include "material/Material.hpp"

namespace rt {

class Scene {
public:
    // EN CLAIR : Scene est le classeur qui possède les objets et référence les
    // matériaux, lumières, caméra et couleur du ciel.
    // ENTRÉES / SORTIES : elle reçoit des éléments construits par le parseur et
    // les fournit au renderer et aux fonctions de shading.
    // APPELÉ PAR : SceneParser, Renderer et Material.
    // PAS À FAIRE ICI : ne pas parser de texte ni calculer une couleur.
    // TODO(C): relier chaque méthode au pipeline propriétaire/non-propriétaire.
    Scene();
    // Les accesseurs évitent de transférer la propriété des ressources.
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;
    void addObject(std::unique_ptr<Hittable> object);
    void addMaterial(const std::shared_ptr<Material>& material);
    const std::shared_ptr<Material>& findMaterial(const std::string& name) const;
    void addLight(const PointLight& light);

    // TODO(C): exposer la collection d'objets sans transférer sa propriété.
    const std::vector<std::unique_ptr<Hittable>>& objects() const noexcept {
        // TODO(C): retourner objects_ en lecture seule.
        static const std::vector<std::unique_ptr<Hittable>> placeholder;
        return placeholder;
    }
    // TODO(C): exposer les lumières en lecture seule.
    const std::vector<PointLight>& lights() const noexcept {
        // TODO(C): retourner lights_ en lecture seule.
        static const std::vector<PointLight> placeholder;
        return placeholder;
    }
    // TODO(C): exposer les paramètres caméra lus par SceneParser.
    const CameraParams& cameraParams() const noexcept {
        // TODO(C): retourner cameraParams_.
        static const CameraParams placeholder;
        return placeholder;
    }
    // TODO(C): exposer la couleur du haut du ciel.
    const Color& background() const noexcept {
        // TODO(C): retourner background_.
        static const Color placeholder;
        return placeholder;
    }
    // TODO(C): copier les paramètres caméra sans construire Camera ici.
    void setCameraParams(const CameraParams&) noexcept {}
    // TODO(C): enregistrer la couleur du haut du ciel.
    void setBackground(const Color&) noexcept {}

private:
    std::vector<std::unique_ptr<Hittable>> objects_;
    std::vector<PointLight> lights_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    CameraParams cameraParams_;
    Color background_;
};

} // namespace rt
