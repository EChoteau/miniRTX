#pragma once

// Fichier : Renderer.hpp
// Rôle : trace récursif, anti-aliasing jitter et rendu en tuiles multithreadées.
// Responsable : Personne C (éclairage/réflexion avec Personne B)
// Dépendances : Image.hpp, Scene.hpp, <atomic>, <random>

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
    public:
    // EN CLAIR : Renderer orchestre le trajet caméra -> rayon -> intersection
    // -> couleur -> image. Il sépare trace (B) et render (C).
    // ENTRÉES / SORTIES : il reçoit Scene et réglages, puis renvoie Image.
    // APPELÉ PAR : main et le benchmark.
    // PAS À FAIRE ICI : ne pas parser la scène ni écrire directement un fichier.

    // TODO(C): relier tileSize et threadCount lors du passage aux tuiles.
    explicit Renderer(RenderSettings settings = {});
    // TODO(C): implémenter le tuilage atomique et le RNG par tuile pour le multi-threading.
    Image render(const Scene& scene) const;

    // Retourne le fond si depth atteint maxDepth, sinon intersection et éclairage.
    Color trace(const Ray& ray, const Scene& scene, int depth) const;

private:
    Color skyColor(const Ray& ray, const Scene& scene) const;
    RenderSettings settings_;
};

} // namespace rt
