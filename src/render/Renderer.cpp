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

#include <limits>      
#include <stdexcept>
#include <random>

namespace rt {

Renderer::Renderer(RenderSettings settings) : settings_(settings) {
    if (settings.width <= 0 || settings.height <= 0)
        throw std::invalid_argument("Width and height must be positive");
    if (settings.samplesPerPixel <= 0)
        throw std::invalid_argument("Samples per pixel must be positive");
    if (settings.maxDepth < 0)
        throw std::invalid_argument("Max depth must be non-negative");
    if (settings.tileSize <= 0)
        throw std::invalid_argument("Tile size must be positive");
    if (settings.threadCount == 0)
        throw std::invalid_argument("Thread count must be at least 1");
}

Image Renderer::render(const Scene& scene) const {
    const CameraParams& cp = scene.cameraParams();
    const double aspect = static_cast<double>(settings_.width) /
                          static_cast<double>(settings_.height);
    const Camera camera(cp.position, cp.lookAt, cp.up, cp.verticalFovDegrees, aspect);

    Image image(settings_.width, settings_.height);

    //rng sans seedbase comme mono tread pour l'instant
    const int spp = settings_.samplesPerPixel;
    std::mt19937 rng(1337u);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int y = 0; y < settings_.height; ++y) {
        for (int x = 0; x < settings_.width; ++x) {
            Color sum{};
            for (int s = 0; s < spp; ++s) {
                // 1 sample : centre du pixel ; sinon position jitterée.
                const double jx = (spp == 1) ? 0.5 : dist(rng);
                const double jy = (spp == 1) ? 0.5 : dist(rng);
                const double u = (x + jx) / settings_.width;
                const double v = 1.0 - (y + jy) / settings_.height;
                sum += trace(camera.getRay(u, v), scene, 0);
            }
            // Moyenne linéaire : ni clamp ni gamma (fait dans savePPM).
            image.setPixel(x, y, sum / static_cast<double>(spp));
        }
    }
    return image;
}

Color Renderer::trace(const Ray& ray, const Scene& scene, int depth) const {

    if (depth >= settings_.maxDepth) return skyColor(ray, scene);

    HitRecord hit;
    if (!scene.hit(ray, RAY_T_MIN, std::numeric_limits<double>::infinity(), hit))
        return skyColor(ray, scene);

    const Color local = computeLocalShading(hit, ray, scene);

    const double r = hit.material ? hit.material->reflectivity : 0.0;
    if (r > 0.0) {
        const Vec3 dir =
            Vec3::reflect(ray.direction().normalized(), hit.normal).normalized();
        const Ray reflected(hit.point, dir);
        const Color refl = trace(reflected, scene, depth + 1);
        return local * (1.0 - r) + refl * r;
    }

    return local;
}

Color Renderer::skyColor(const Ray& ray, const Scene& scene) const {
    const double t = 0.5 * (ray.direction().normalized().y() + 1.0);
    return Color{1.0, 1.0, 1.0} * (1.0 - t) + scene.background() * t;
}

}