#pragma once

// Fichier : Image.hpp
// Rôle : buffer RGB et export PPM P6 avec gamma 2.2.
// Responsable : Personne C
// Dépendances : Color.hpp, <string>, <vector>

#include <string>
#include <vector>
#include "core/Color.hpp"

namespace rt {

/// Stocke une image RGB et l'exporte au format PPM.
class Image {
public:
    /// Construit une image de dimensions positives.
    Image(int width, int height);

    /// Retourne la largeur de l'image.
    int width() const noexcept;

    /// Retourne la hauteur de l'image.
    int height() const noexcept;

    /// Définit la couleur du pixel indiqué.
    void setPixel(int x, int y, const Color& color);

    /// Retourne la couleur du pixel indiqué.
    Color pixel(int x, int y) const;

    /// Écrit l'image au format PPM binaire avec correction gamma.
    void savePPM(const std::string& path) const;

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
};

} // namespace rt
