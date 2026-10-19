#pragma once

// Fichier : Sphere.hpp
// Rôle : primitive sphérique et interface d'intersection quadratique.
// Responsable : Personne A
// Dépendances : Hittable.hpp, Material.hpp
// TODO(A): lancer std::invalid_argument si radius <= EPSILON ou si le matériau
// est nul ; choisir ensuite la racine valide la plus proche.

#include <memory>
#include "geometry/Hittable.hpp"
#include "material/Material.hpp"

namespace rt {

class Sphere final : public Hittable {
public:
    // EN CLAIR : une sphère est définie par un centre, un rayon et un matériau.
    // TODO(A): vérifier le rayon/matériau puis résoudre l'équation quadratique.
    // Le matériau est partagé car plusieurs objets peuvent référencer le même matériau.
    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material);
    bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
    const Vec3& center() const noexcept {
        // TODO(A): retourner le centre géométrique.
        static const Vec3 placeholder;
        return placeholder;
    }
    double radius() const noexcept {
        // TODO(A): retourner le rayon validé.
        return 0.0;
    }

private:
    Vec3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

} // namespace rt
