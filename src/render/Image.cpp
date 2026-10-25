// Fichier : Image.cpp
// Rôle : squelette du buffer et de l'export PPM avec correction gamma.
// Responsable : Personne C
// Dépendances : Image.hpp, <cmath>, <fstream>, <stdexcept>
// CHAÎNE : Renderer accumule une couleur linéaire -> Image la stocke ->
// savePPM applique clamp et gamma -> le fichier devient visualisable.

#include "render/Image.hpp"
#include <stdexcept>
#include <iostream>

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

namespace rt {

Image::Image(int width, int height)
    // Verrifie les dimensions et réserve width*height en size_t
    : width_(width), height_(height), pixels_() {

    
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }

    pixels_.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

    width_ = width;
    height_ = height;
}

int Image::width() const noexcept {
    return width_;
}

int Image::height() const noexcept {
    return height_;
}

void Image::setPixel(int x, int y, const Color& color) {
    // Vérifie x/y et stocke les pixels_[y * width_ + x]
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Coordinates out of bounds");
    }
    pixels_[y * width_ + x] = color;
}

Color Image::pixel(int x, int y) const {
    // Verifie les coordonnées puis retourne les pixels_[y * width_ + x]
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Coordinates out of bounds");
    }
    return pixels_[y * width_ + x];
}


// Écrit l'image au format PPM binaire (P6)
// Chaque composante subit : clampColor -> gamma (pow 1/2.2) -> toRgb8
// Lève std::runtime_error si le fichier ne peut pas être ouvert ou écrit
void Image::savePPM(const std::string& path) const {
    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Failed to open file for writing: " + path);
    }

    const std::string header =
        "P6\n" + std::to_string(width_) + " " + std::to_string(height_) + "\n255\n";

    // Taille finale connue donc une seule allocation : écriture directe par index
    std::vector<std::uint8_t> buffer(pixels_.size() * 3);

    constexpr double invGamma = 1.0 / 2.2;
    std::size_t i = 0;

    // Parcours linéaire de pixels_ : même ordre que pixel(y, x) avec y=0 en haut,
    // et pas de calcul d'index ni de vérification de bornes par pixel.
    for (const Color& pixelColor : pixels_) {
        // 1. clamp : évite les valeurs négatives (NaN avec pow) et > 1
        Color c = clampColor(pixelColor);

        // 2. gamma : linéaire -> espace d'affichage
        c = Color{std::pow(c.x(), invGamma),
                  std::pow(c.y(), invGamma),
                  std::pow(c.z(), invGamma)};

        // 3. quantification 8 bits en dernier (étape destructive)
        const auto rgb = toRgb8(c);
        buffer[i++] = rgb[0];
        buffer[i++] = rgb[1];
        buffer[i++] = rgb[2];
    }

    output.write(header.data(), static_cast<std::streamsize>(header.size()));
    output.write(reinterpret_cast<const char*>(buffer.data()),
                 static_cast<std::streamsize>(buffer.size()));
    output.flush();

    if (!output) {
        throw std::runtime_error("Failed to write to file: " + path);
    }
}


} // namespace rt
