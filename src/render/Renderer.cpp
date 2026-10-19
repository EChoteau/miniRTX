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

Color Renderer::trace(const Ray&, const Scene& scene, int depth) const {
    // TODO(B): depth >= maxDepth ou absence d'intersection -> skyColor ; sinon
    // shading local, puis réflexion avec tMin=RAY_T_MIN et depth+1. Ne jamais
    // clamper dans trace/shading : le clamp et le gamma sont réservés à l'export.
    return Color{};
}

Color Renderer::skyColor(const Ray&) const {
    // TODO(B/C): calculer le dégradé vertical demandé à partir de la direction.
    return Color{};
}

} // namespace rt
