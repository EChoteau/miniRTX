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
    Scene();
    // Les accesseurs évitent de transférer la propriété des ressources.
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const;
    void addObject(std::unique_ptr<Hittable> object);
    void addMaterial(const std::shared_ptr<Material>& material);
    const std::shared_ptr<Material>& findMaterial(const std::string& name) const;
    void addLight(const PointLight& light);

    const std::vector<std::unique_ptr<Hittable>>& objects() const noexcept { return objects_; }
    const std::vector<PointLight>& lights() const noexcept { return lights_; }
    const Camera& camera() const noexcept { return camera_; }
    const Color& background() const noexcept { return background_; }
    void setCamera(const Camera& camera) noexcept { camera_ = camera; }
    void setBackground(const Color& color) noexcept { background_ = color; }

private:
    std::vector<std::unique_ptr<Hittable>> objects_;
    std::vector<PointLight> lights_;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
    Camera camera_;
    Color background_;
};

} // namespace rt
