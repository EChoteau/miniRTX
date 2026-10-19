#pragma once

// Fichier : Material.hpp
// Rôle : paramètres de réflexion locale et miroir d'une surface.
// Responsable : Personne B
// Dépendances : Color.hpp, HitRecord.hpp (déclarations), Ray.hpp
// TODO : TODO(B): implémenter le calcul Phong dans Material.cpp sans dépendance circulaire.

#include <string>
#include "core/Color.hpp"

namespace rt {

struct HitRecord;
class Ray;
class Scene;
struct PointLight;

struct Material {
    // Nom utilisé par SceneParser pour relier les objets à leur matériau.
    std::string name;
    // Couleur intrinsèque RGB dans [0, 1].
    Color albedo{1.0, 1.0, 1.0};
    // Coefficients Phong : ambiant, diffus, spéculaire.
    double ambient{0.1};
    double diffuse{0.8};
    double specular{0.2};
    // Exposant de brillance ; éviter une valeur négative.
    double shininess{32.0};
    // Part de réflexion récursive dans [0, 1].
    double reflectivity{0.0};
};

// Calcule ambiant + diffus + spéculaire pour toutes les lumières non occultées.
Color computeLocalShading(const HitRecord& hit, const Ray& viewRay, const Scene& scene);

// Lance un rayon depuis un point décalé de EPSILON vers la lumière.
bool isInShadow(const Vec3& point, const PointLight& light, const Scene& scene);

} // namespace rt
