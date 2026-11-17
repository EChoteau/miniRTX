#pragma once

// Fichier : Utils.hpp
// Rôle : constantes et fonctions numériques communes au moteur.
// Responsable : Personne A
// Dépendances : <algorithm>, <cmath>, <limits>
// Note : EPSILON et RAY_T_MIN sont adaptés à des scènes d'ordre de grandeur ~1-100.

#include <algorithm>
#include <cmath>
#include <limits>

namespace rt {

inline constexpr double EPSILON = 1e-6;
// tMin de TOUS les rayons secondaires (ombre, réflexion) pour éviter l'acné
// d'ombre ; EPSILON (1e-6) sert aux tests de dégénérescence (longueur nulle,
// dénominateur ~ 0).
inline constexpr double RAY_T_MIN = 1e-4;
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double INFINITY_VALUE = std::numeric_limits<double>::infinity();

// Borne value dans [minValue, maxValue] (on suppose minValue <= maxValue).
inline double clamp(double value, double minValue, double maxValue) noexcept {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

// Convertit des degrés en radians : degrés * PI / 180.
inline double degreesToRadians(double degrees) noexcept {
    return degrees * PI / 180.0;
}

} // namespace rt
