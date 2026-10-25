#pragma once

// Fichier : Image.hpp
// Rôle : buffer RGB et export PPM P6 avec gamma 2.2.
// Responsable : Personne C
// Dépendances : Color.hpp, <string>, <vector>

#include <string>
#include <vector>
#include "core/Color.hpp"

namespace rt {

class Image {
public:
    // EN CLAIR : Image est une grille mémoire de couleurs linéaires avant
    // export. Chaque pixel est adressé par (x,y), avec y=0 en haut.
    // ENTRÉES / SORTIES : elle reçoit dimensions et couleurs, puis écrit PPM.
    // APPELÉ PAR : Renderer puis main.
    // PAS À FAIRE ICI : ne pas appliquer gamma pendant le shading.
    Image(int width, int height);
    int width() const noexcept;
    int height() const noexcept;
    void setPixel(int x, int y, const Color& color);
    Color pixel(int x, int y) const;
    void savePPM(const std::string& path) const;

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
};

} // namespace rt
