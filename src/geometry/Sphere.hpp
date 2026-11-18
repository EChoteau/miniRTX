#pragma once

// Fichier : Sphere.hpp
// Rôle : primitive sphérique et interface d'intersection quadratique.
// Responsable : Personne A
// Dépendances : Hittable.hpp, Material.hpp
// Lance std::invalid_argument si radius <= EPSILON ou si le matériau est nul.
// hit() retient la racine valide la plus proche dans ]tMin, tMax[.

#include <memory>
#include "geometry/Hittable.hpp"
#include "material/Material.hpp"

namespace rt {

class Sphere final : public Hittable {
public:
    // EN CLAIR : une sphère est définie par un centre, un rayon et un matériau.
    // Le matériau est partagé car plusieurs objets peuvent référencer le même matériau.
    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material);
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
    const Vec3& center() const noexcept { return center_; }
    double radius() const noexcept { return radius_; }

private:
    Vec3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

} // namespace rt
