#pragma once

// Fichier : Renderer.hpp
// Rôle : trace récursif, anti-aliasing jitter et rendu en tuiles multithreadées.
// Responsable : Personne C (éclairage/réflexion avec Personne B)
// Dépendances : Image.hpp, Scene.hpp, <atomic>, <random>
// TODO : TODO(C): implémenter compteur atomique de tuiles et RNG local à chaque worker.
// TODO : TODO(B): intégrer shading Phong, ombre et réflexion jusqu'à maxDepth.

#include <atomic>
#include <random>
#include "render/Image.hpp"
#include "scene/Scene.hpp"

namespace rt {

struct RenderSettings {
    int width{800};
    int height{450};
    int samplesPerPixel{1};
    int maxDepth{5};
    unsigned threadCount{1};
    int tileSize{32};
};

class Renderer {
public:
    explicit Renderer(RenderSettings settings = {});
    Image render(const Scene& scene) const;
    // Retourne le fond si depth atteint maxDepth, sinon intersection et éclairage.
    Color trace(const Ray& r, const Scene& scene, int depth) const;

private:
    RenderSettings settings_;
};

} // namespace rt
