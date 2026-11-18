#pragma once

// Fichier : HitRecord.hpp
// Rôle : informations produites par une intersection rayon/objet.
// Responsable : Personne A
// Dépendances : Ray.hpp, Vec3.hpp
// Note : material pointe vers un Material possédé (shared_ptr) par Scene et par
// la primitive ; il reste valide tant que la Scene existe.

#include "core/Ray.hpp"

namespace rt {

struct Material;

struct HitRecord {
    // Point d'intersection et normale orientée vers le rayon incident.
    Vec3 point{};
    Vec3 normal{};
    // Paramètre du rayon ; le plus petit t valide est retenu.
    double t{0.0};
    // Vrai lorsque le rayon touche la face externe de l'objet.
    bool frontFace{true};
    // Non propriétaire ; le matériau appartient à Scene.
    const Material* material{nullptr};

    // outwardNormal doit être unitaire et pointer vers l'EXTÉRIEUR de l'objet.
    // Si le rayon arrive de face (dot < 0) on garde la normale, sinon on la
    // retourne : la normale stockée est toujours orientée contre le rayon.
    void setFaceNormal(const Ray& r, const Vec3& outwardNormal) noexcept {
        frontFace = dot(r.direction(), outwardNormal) < 0.0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

} // namespace rt
