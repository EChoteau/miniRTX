// Fichier : Renderer.cpp
// Rôle : squelette de boucle de rendu, trace récursif et tuilage atomique.
// Responsable : Personne C
// Dépendances : Renderer.hpp, Material.hpp
// TODO : TODO(C): distribuer les tuiles et accumuler les samples avec un RNG par thread.

#include "render/Renderer.hpp"
#include "material/Material.hpp"

namespace rt {

Renderer::Renderer(RenderSettings settings) : settings_(settings) {}

Image Renderer::render(const Scene& scene) const {
    // TODO(C): créer Image, lancer les rayons et retourner le buffer rempli.
    return Image(settings_.width, settings_.height);
}

Color Renderer::trace(const Ray&, const Scene& scene, int depth) const {
    // TODO(B): arrêter à maxDepth, intersecter, appeler computeLocalShading et réfléchir.
    if (depth >= settings_.maxDepth) {
        return scene.background();
    }
    return scene.background();
}

} // namespace rt
