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
// tMin de TOUS les rayons secondaires (ombre, réflexion) pour éviter l'acné
// d'ombre ; EPSILON (1e-6) sert aux tests de dégénérescence (longueur nulle,
// dénominateur ~ 0).
inline constexpr double RAY_T_MIN = 1e-4;
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double INFINITY_VALUE = std::numeric_limits<double>::infinity();

// Borne value dans [minValue, maxValue].
// TODO(A): borner value entre minValue et maxValue.
inline double clamp(double, double, double) noexcept {
    return 0.0;
}

// Convertit des degrés en radians : degrés * PI / 180.
// TODO(A): convertir des degrés en radians avec PI.
inline double degreesToRadians(double) noexcept {
    return 0.0;
}

} // namespace rt
