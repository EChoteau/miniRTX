#pragma once

// Fichier : HitRecord.hpp
// Rôle : informations produites par une intersection rayon/objet.
// Responsable : Personne A
// Dépendances : Ray.hpp, Vec3.hpp
// TODO : TODO(A): vérifier la durée de vie du pointeur material avec Scene.

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

    // Inverse la normale si le rayon vient de l'intérieur.
    void setFaceNormal(const Ray& ray, const Vec3& outwardNormal) noexcept {
        frontFace = dot(ray.direction(), outwardNormal) < 0.0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

} // namespace rt
