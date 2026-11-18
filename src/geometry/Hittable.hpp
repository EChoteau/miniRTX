#pragma once

// Fichier : Hittable.hpp
// Rôle : interface abstraite commune à toutes les primitives intersectables.
// Responsable : Personne A
// Dépendances : HitRecord.hpp, Ray.hpp
// TODO : TODO(A): envisager une boîte englobante pour une future BVH.

#include "geometry/HitRecord.hpp"

namespace rt {

class Hittable {
public:
    // EN CLAIR : Hittable est le contrat commun de toutes les formes visibles.
    // ENTRÉES / SORTIES : un rayon et un intervalle entrent, HitRecord sort si
    // la forme est touchée.
    // APPELÉ PAR : Scene::hit.
    // PAS À FAIRE ICI : ne pas connaître le type concret de l'objet.
    virtual ~Hittable() = default;
    // Cherche l'intersection la plus proche avec t dans ]tMin, tMax[ ; remplit rec
    // (point, normale unitaire opposée au rayon, t, frontFace, material) si vrai.
    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};

} // namespace rt
