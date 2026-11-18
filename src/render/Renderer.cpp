// Fichier : Renderer.cpp
// Rôle : squelette de boucle de rendu, trace récursif et tuilage atomique.
// Responsable : Personne C
// Dépendances : Renderer.hpp, Material.hpp
// TODO : TODO(C): distribuer les tuiles et accumuler les samples avec un RNG par thread.
// CHAÎNE DESCENDANTE : main -> SceneParser -> render -> Camera -> trace.
// CHAÎNE REMONTANTE : trace -> Scene::hit -> Material -> Image -> savePPM.

#include "render/Renderer.hpp"
#include "render/ThreadPool.hpp"
#include "material/Material.hpp"

namespace rt {

Renderer::Renderer(RenderSettings settings)
    // TODO(C): valider les réglages de rendu.
    : settings_(settings) {}

Image Renderer::render(const Scene& scene) const {
    // TODO(C): construire la Camera ici avec aspect = double(width)/height.
    // TODO(C): tuiles 32x32, RNG mt19937 par tuile seedé baseSeed + tileIndex,
    // mapping u=(x+jx)/width, v=1-(y+jy)/height, puis moyenne linéaire sans
    // clamp ; pour un sample jx=jy=0.5.
    return Image(settings_.width, settings_.height);
}

Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {
    // La profondeur borne la récursion avant de lancer un rayon supplémentaire.
    if (depth >= settings_.maxDepth) {
        return skyColor(ray, scene);
    }

    HitRecord hit;
    if (!scene.hit(ray, RAY_T_MIN, INFINITY_VALUE, hit)) {
        // Un rayon qui ne rencontre aucun objet voit le dégradé du ciel.
        return skyColor(ray, scene);
    }

    // Le matériau fournit la couleur directe, déjà calculée sans clamp.
    const Color local = computeLocalShading(hit, ray, scene);
    if (hit.material == nullptr || hit.material->reflectivity <= 0.0) {
        return local;
    }

    // Le rayon réfléchi repart du point touché ; RAY_T_MIN évitera
    // l'auto-intersection lorsque la scène testera ce rayon secondaire.
    const Ray reflectedRay(
        hit.point, reflect(ray.direction(), hit.normal).normalized());
    const Color reflected = trace(reflectedRay, scene, depth + 1);
    const double reflectivity = hit.material->reflectivity;

    // Le mélange conserve la couleur locale et ajoute la part miroir.
    return (1.0 - reflectivity) * local + reflectivity * reflected;
}

Color Renderer::skyColor(const Ray& ray, const Scene& scene) const {
    // t varie de 0 (horizon) à 1 (haut du ciel) selon la direction du rayon.
    const double directionY = ray.direction().normalized().y();
    const double t = 0.5 * (directionY + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * scene.background();
}

} // namespace rt
