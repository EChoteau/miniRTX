#pragma once

// Fichier : Renderer.hpp
// Rôle : trace récursif, anti-aliasing jitter et rendu en tuiles multithreadées.
// Responsable : Personne C (éclairage/réflexion avec Personne B)
// Dépendances : Image.hpp, Scene.hpp, <atomic>, <random>
// TODO(C): compteur atomique de tuiles, RNG par tuile et accumulation linéaire.
// TODO(B): intégrer shading Phong, ombre et réflexion jusqu'à maxDepth.

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
    // EN CLAIR : Renderer orchestre le trajet caméra -> rayon -> intersection
    // -> couleur -> image. Il sépare trace (B) et render (C).
    // ENTRÉES / SORTIES : il reçoit Scene et réglages, puis renvoie Image.
    // APPELÉ PAR : main et le benchmark.
    // PAS À FAIRE ICI : ne pas parser la scène ni écrire directement un fichier.
    // TODO(C): relier les réglages aux étapes du pipeline.
    // TODO(C): conserver les réglages et vérifier dimensions, samples, profondeur.
    explicit Renderer(RenderSettings settings = {});
    // TODO(C): créer l'image, partager les tuiles atomiquement et écrire chaque pixel.
    Image render(const Scene& scene) const;
    // Retourne le fond si depth atteint maxDepth, sinon intersection et éclairage.
    // TODO(B): gérer profondeur max, miss, shading local et réflexion.
    Color trace(const Ray& r, const Scene& scene, int depth) const;

private:
    // TODO(B/C): skyColor = (1-t)*blanc + t*background, avec
    // t = 0.5 * (ray.direction.normalized().y() + 1) ; le background est le
    // haut du ciel. L'utiliser en absence d'intersection et à profondeur max.
    Color skyColor(const Ray& ray) const;
    RenderSettings settings_;
};

} // namespace rt
