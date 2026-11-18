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

/// Conteneur des objets, matériaux, lumières et paramètres d'une scène.
class Scene {
public:
    /// Initialise une scène avec une caméra par défaut et un fond noir.
    Scene();

    /// Recherche l'intersection la plus proche avec les objets de la scène.
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;

    /// Ajoute un objet en transférant sa propriété à la scène.
    void addObject(std::unique_ptr<Hittable> object);

    /// Ajoute un matériau, dont le nom doit être unique.
    void addMaterial(const std::shared_ptr<Material>& material);

    /// Recherche un matériau par son nom.
    const std::shared_ptr<Material>& findMaterial(const std::string& name) const;

    /// Ajoute une lumière à la scène.
    void addLight(const PointLight& light);

    /// Retourne les objets sans en transférer la propriété.
    const std::vector<std::unique_ptr<Hittable>>& objects() const noexcept { return objects_; }

    /// Retourne les lumières en lecture seule.
    const std::vector<PointLight>& lights() const noexcept { return lights_; }

    /// Retourne les paramètres de caméra.
    const CameraParams& cameraParams() const noexcept { return cameraParams_; }

    /// Retourne la couleur du fond.
    const Color& background() const noexcept { return background_; }

    /// Définit les paramètres de caméra.
    void setCameraParams(const CameraParams& p) noexcept { cameraParams_ = p; }

    /// Définit la couleur du fond.
    void setBackground(const Color& c) noexcept { background_ = c; }

private:
    std::vector<std::unique_ptr<Hittable>> objects_;
    std::vector<PointLight> lights_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    CameraParams cameraParams_;
    Color background_;
};

} // namespace rt
