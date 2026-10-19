// Fichier : Image.cpp
// Rôle : squelette du buffer et de l'export PPM avec correction gamma.
// Responsable : Personne C
// Dépendances : Image.hpp, <cmath>, <fstream>, <stdexcept>
// TODO : TODO(C): écrire l'en-tête P6 et trois octets par pixel.

#include "render/Image.hpp"
#include <stdexcept>

namespace rt {

Image::Image(int width, int height)
    : width_(width), height_(height), pixels_(static_cast<std::size_t>(width * height)) {}

void Image::setPixel(int, int, const Color&) {
    // TODO(C): vérifier x/y et stocker pixels_[y * width_ + x].
}

Color Image::pixel(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("pixel coordinates outside image");
    }
    // TODO(C): retourner pixels_[y * width_ + x].
    return Color{};
}

void Image::savePPM(const std::string& path) const {
    // TODO(C): appliquer gamma = 1/2.2 et gérer les erreurs d'ouverture/écriture.
    (void)path;
}

} // namespace rt
