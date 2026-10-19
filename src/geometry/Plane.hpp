#pragma once

// Fichier : Plane.hpp
// Rôle : plan infini défini par un point et une normale.
// Responsable : Personne A
// Dépendances : Hittable.hpp, Material.hpp
// TODO(A): lancer std::invalid_argument si la normale a une longueur <=
// EPSILON ou si le matériau est nul ; éviter toute division par zéro.

#include <memory>
#include "geometry/Hittable.hpp"
#include "material/Material.hpp"

namespace rt {

class Plane final : public Hittable {
public:
    // EN CLAIR : un plan est une surface infinie définie par un point et une
    // normale. TODO(A): rejeter les normales nulles et calculer t.
    Plane(const Vec3& point, const Vec3& normal, std::shared_ptr<Material> material);
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    Vec3 point_;
    Vec3 normal_;
    std::shared_ptr<Material> material_;
};

} // namespace rt
