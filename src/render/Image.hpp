#pragma once

// Fichier : Image.hpp
// Rôle : buffer RGB et export PPM P6 avec gamma 2.2.
// Responsable : Personne C
// Dépendances : Color.hpp, <string>, <vector>
// TODO : TODO(C): protéger les coordonnées hors image et vérifier les erreurs d'écriture.

#include <string>
#include <vector>
#include "core/Color.hpp"

namespace rt {

class Image {
public:
    Image(int width, int height);
    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    void setPixel(int x, int y, const Color& color);
    Color pixel(int x, int y) const;
    // Écrit un PPM binaire P6 ; applique clamp puis pow(component, 1/2.2).
    void savePPM(const std::string& path) const;

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
};

} // namespace rt
