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
    // TODO(A): garder l'interface minimale pour Sphere, Plane et Triangle.
    // TODO(A): détruire proprement la primitive via le pointeur de base.
    virtual ~Hittable() = default;
    // Cherche une intersection dans l'intervalle fermé conceptuellement [tMin, tMax].
    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
};

} // namespace rt
