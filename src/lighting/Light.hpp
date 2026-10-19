#pragma once

// Fichier : Light.hpp
// Rôle : source ponctuelle utilisée par le modèle de Phong.
// Responsable : Personne B
// Dépendances : Color.hpp, Vec3.hpp
// TODO : TODO(B): ajouter éventuellement des types de lumière supplémentaires.

#include "core/Color.hpp"

namespace rt {

struct PointLight {
    // Position de la source dans le repère monde.
    Vec3 position;
    // Couleur de la lumière, habituellement dans [0, 1].
    Color color{1.0, 1.0, 1.0};
    // Intensité multiplicative non négative.
    double intensity{1.0};
};

} // namespace rt
