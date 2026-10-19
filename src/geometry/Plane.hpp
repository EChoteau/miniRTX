#pragma once

// Fichier : Plane.hpp
// Rôle : plan infini défini par un point et une normale.
// Responsable : Personne A
// Dépendances : Hittable.hpp, Material.hpp
// TODO : TODO(A): refuser une normale quasi nulle et éviter la division par zéro.

#include <memory>
#include "geometry/Hittable.hpp"
#include "material/Material.hpp"

namespace rt {

class Plane final : public Hittable {
public:
    Plane(const Vec3& point, const Vec3& normal, std::shared_ptr<Material> material) noexcept;
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    Vec3 point_;
    Vec3 normal_;
    std::shared_ptr<Material> material_;
};

} // namespace rt
