// Fichier : Image.cpp
// Rôle : squelette du buffer et de l'export PPM avec correction gamma.
// Responsable : Personne C
// Dépendances : Image.hpp, <cmath>, <fstream>, <stdexcept>
// TODO : TODO(C): écrire l'en-tête P6 et trois octets par pixel.
// CHAÎNE : Renderer accumule une couleur linéaire -> Image la stocke ->
// savePPM applique clamp et gamma -> le fichier devient visualisable.

#include "render/Image.hpp"
#include <stdexcept>

namespace rt {

Image::Image(int width, int height)
    // TODO(C): vérifier les dimensions et réserver width*height en size_t.
    : width_(width), height_(height), pixels_() {
}

void Image::setPixel(int, int, const Color&) {
    // TODO(C): vérifier x/y et stocker pixels_[y * width_ + x].
}

Color Image::pixel(int x, int y) const {
    // TODO(C): vérifier les coordonnées puis retourner pixels_[y * width_ + x].
    (void)x;
    (void)y;
    return Color{};
}

void Image::savePPM(const std::string& path) const {
    // TODO(C): écrire P6\n<w> <h>\n255\n, appliquer clamp -> pow -> toRgb8
    // dans cet ordre et lever runtime_error en cas d'ouverture ou d'écriture.
    (void)path;
}

} // namespace rt
