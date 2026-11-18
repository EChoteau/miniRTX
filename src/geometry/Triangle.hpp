#pragma once

// Fichier : Triangle.hpp
// Rôle : primitive triangulaire optionnelle utilisant Möller-Trumbore.
// Responsable : Personne A (OPTIONNEL)
// Dépendances : Hittable.hpp, Material.hpp
// Lance std::invalid_argument si le triangle est dégénéré ou si le matériau est
// nul. hit() utilise Möller-Trumbore ; la normale extérieure suit A -> B -> C.

#include <memory>
#include "geometry/Hittable.hpp"
#include "material/Material.hpp"

namespace rt {

class Triangle final : public Hittable {
public:
    // EN CLAIR : un triangle est la surface finie entre trois sommets.
    Triangle(const Vec3& a, const Vec3& b, const Vec3& c,
             std::shared_ptr<Material> material);
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    Vec3 a_;
    Vec3 b_;
    Vec3 c_;
    std::shared_ptr<Material> material_;
};

} // namespace rt
