#pragma once

// Fichier : Color.hpp
// Rôle : couleur RGB flottante, clamp et conversion PPM 8 bits.
// Responsable : Personne A
// Dépendances : <array>, <cstdint>, Vec3.hpp
// TODO(A): le gamma est appliqué UNIQUEMENT dans Image::savePPM. Color ne fait
// que le clamp et la conversion en 8 bits.

#include <array>
#include <cstdint>
#include "core/Vec3.hpp"

namespace rt {

using Color = Vec3;

// TODO(A): borner chaque composante dans [0,1]. Le gamma reste dans Image.
inline Color clampColor(const Color&) noexcept {
    return Color{};
}

// TODO(A): convertir une couleur déjà clampée en trois composantes 8 bits.
inline std::array<std::uint8_t, 3> toRgb8(const Color&) noexcept {
    return {0, 0, 0};
}

} // namespace rt
