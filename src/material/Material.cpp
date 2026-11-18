// Fichier : Material.cpp
// Rôle : squelette du modèle de Phong et des ombres.
// Responsable : Personne B
// Dépendances : Material.hpp, Scene.hpp, Light.hpp
// TODO : TODO(B): additionner ambiant, diffus max(dot(n,l),0) et spéculaire.
// CHAÎNE : HitRecord arrive de Scene -> Material évalue Phong et les ombres ->
// Renderer ajoute éventuellement le rayon réfléchi.

#include "material/Material.hpp"
#include "scene/Scene.hpp"
#include "lighting/Light.hpp"
#include <algorithm>
#include <cmath>

namespace rt {

Color computeLocalShading(const HitRecord& hit, const Ray& viewRay, const Scene& scene) {
    const ShadingTerms terms = computeShadingTerms(hit, viewRay, scene);
    return terms.ambientDiffuse + terms.specular;
}

ShadingTerms computeShadingTerms(const HitRecord& hit, const Ray& viewRay, const Scene& scene) {
    const Material& material = *hit.material;
    // N et V doivent être unitaires pour que les produits scalaires
    // représentent directement des cosinus d'angles.
    const Vec3 normal = hit.normal.normalized();
    const Vec3 view = (-viewRay.direction()).normalized();

    // L'ambiant ne dépend pas des lumières et n'est ajouté qu'une seule fois.
    ShadingTerms result{material.ambient * material.albedo, Color{}};

    for (const PointLight& light : scene.lights()) {
        if (isInShadow(hit.point, light, scene)) {
            // Une ombre retire les contributions de cette lumière uniquement.
            continue;
        }

        // L est calculé après le test d'ombre : il sert à mesurer l'angle
        // entre la surface et la lumière.
        const Vec3 toLight = light.position - hit.point;
        const double lightDistance = toLight.length();
        if (lightDistance <= EPSILON) {
            continue;
        }

        const Vec3 lightDirection = toLight / lightDistance;
        // Une lumière derrière la surface ne produit ni diffus ni spéculaire.
        const double diffuseFactor =
            std::max(Vec3::dot(normal, lightDirection), 0.0);
        // La couleur et l'intensité de la lampe modulent diffus et spéculaire.
        const Color lightColor = light.color * light.intensity;
        result.ambientDiffuse +=
            lightColor * (material.diffuse * diffuseFactor * material.albedo);

        if (diffuseFactor > 0.0) {
            // Le spéculaire ne s'ajoute que si la lumière atteint la face.
            const Vec3 reflectedLight = reflect(-lightDirection, normal);
            const double specularBase =
                std::max(Vec3::dot(reflectedLight, view), 0.0);
            // La base de pow est bornée pour éviter une contribution négative.
            const double specularFactor =
                std::pow(specularBase, std::max(material.shininess, 0.0));
            const double specularContribution =
                material.specular * specularFactor;
            // Le spéculaire est une intensité blanche ajoutée aux trois canaux.
            result.specular += lightColor * Color(
                specularContribution, specularContribution, specularContribution);
        }
    }

    return result;
}

bool isInShadow(const Vec3& point, const PointLight& light, const Scene& scene) {
    // Le rayon d'ombre suit le segment entre le point touché et la lumière.
    const Vec3 toLight = light.position - point;
    const double lightDistance = toLight.length();
    if (lightDistance <= EPSILON) {
        // Il n'y a pas de direction exploitable si le point est sur la lumière.
        return false;
    }

    // La normalisation fait de t une distance mesurable sur le rayon.
    const Ray shadowRay(point, toLight / lightDistance);
    HitRecord shadowHit;
    // On ignore la surface de départ et tout ce qui est derrière la lumière.
    return scene.hit(
        shadowRay, RAY_T_MIN, lightDistance - RAY_T_MIN, shadowHit);
}

} // namespace rt
