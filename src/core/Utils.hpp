#pragma once

// Fichier : Utils.hpp
// Rôle : constantes et fonctions numériques communes au moteur.
// Responsable : Personne A
// Dépendances : <algorithm>, <cmath>, <limits>
// TODO : TODO(A): vérifier les choix d'epsilon selon l'échelle des scènes.

#include <algorithm>
#include <cmath>
#include <limits>

namespace rt {

inline constexpr double EPSILON = 1e-6;
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double INFINITY_VALUE = std::numeric_limits<double>::infinity();

// Borne value dans [minValue, maxValue].
inline double clamp(double value, double minValue, double maxValue) noexcept {
    return std::max(minValue, std::min(value, maxValue));
}

// Convertit des degrés en radians : degrés * PI / 180.
inline double degreesToRadians(double degrees) noexcept {
    return degrees * PI / 180.0;
}

} // namespace rt
