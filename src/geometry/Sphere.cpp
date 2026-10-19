// Fichier : Sphere.cpp
// Rôle : squelette de l'intersection rayon-sphère.
// Responsable : Personne A
// Dépendances : Sphere.hpp
// TODO : TODO(A): résoudre le discriminant et remplir HitRecord.
// CHAÎNE : Renderer fournit un Ray -> Scene appelle hit -> Sphere calcule ses
// racines -> HitRecord repart vers le shading.

#include "geometry/Sphere.hpp"
#include <utility>

namespace rt {

Sphere::Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material)
    // TODO(A): valider radius et material avant de conserver les données.
    : center_(center), radius_(radius), material_(std::move(material)) {}

bool Sphere::hit(const Ray&, double, double, HitRecord&) const {
    // TODO(A): implémenter la formule quadratique et setFaceNormal.
    return false;
}

} // namespace rt
