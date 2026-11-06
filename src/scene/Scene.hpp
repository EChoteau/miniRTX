#pragma once

// Fichier : Scene.hpp
// Rôle : propriété des objets, matériaux, lumières, caméra et fond.
// Responsable : Personne C
// Dépendances : Camera.hpp, Hittable.hpp, Light.hpp, Material.hpp

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
    
    Scene();
    
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;
    void addObject(std::unique_ptr<Hittable> object);
    void addMaterial(const std::shared_ptr<Material>& material);
    const std::shared_ptr<Material>& findMaterial(const std::string& name) const;
    void addLight(const PointLight& light);

    // Expose la collection d'objets sans transférer sa propriété.
    const std::vector<std::unique_ptr<Hittable>>& objects() const noexcept { return objects_; }

    // Expose les lumières en lecture seule.
    const std::vector<PointLight>& lights() const noexcept { return lights_; }

    // Expose les paramètres caméra lus par SceneParser.
    const CameraParams& cameraParams() const noexcept { return cameraParams_; }

    // Expose la couleur du haut du ciel.
    const Color& background() const noexcept { return background_; }

    // Copier les paramètres caméra sans construire Camera ici.
    void setCameraParams(const CameraParams& p) noexcept { cameraParams_ = p; }
    
    // Enregistrer la couleur du haut du ciel.
    void setBackground(const Color& c) noexcept { background_ = c; }
   

private:
    std::vector<std::unique_ptr<Hittable>> objects_;
    std::vector<PointLight> lights_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    CameraParams cameraParams_;
    Color background_;
};

} // namespace rt
