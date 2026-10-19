#pragma once

// Fichier : Image.hpp
// Rôle : buffer RGB et export PPM P6 avec gamma 2.2.
// Responsable : Personne C
// Dépendances : Color.hpp, <string>, <vector>
// TODO(C): constructeur invalid_argument si width/height <= 0 ; calculer la
// taille en size_t. setPixel/pixel hors image -> std::out_of_range.

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
    // TODO(C): décrire le cycle dimensions -> pixels -> export.
    Image(int width, int height);
    // TODO(C): retourner les dimensions.
    int width() const noexcept { return 0; }
    int height() const noexcept { return 0; }
    void setPixel(int x, int y, const Color& color);
    Color pixel(int x, int y) const;
    // TODO(C): P6 binaire ; par composante clampColor -> pow(c, 1/2.2) ->
    // toRgb8, puis vérifier ouverture et état du flux après écriture.
    void savePPM(const std::string& path) const;

private:
    int width_;
    int height_;
    std::vector<Color> pixels_;
};

} // namespace rt
