#pragma once

// Fichier : Color.hpp
// Rôle : couleur RGB flottante, clamp et conversion PPM 8 bits.
// Responsable : Personne A
// Dépendances : <array>, <cstdint>, Vec3.hpp
// Le gamma est appliqué UNIQUEMENT dans Image::savePPM. Color ne fait que le
// clamp et la conversion en 8 bits.

#include <array>
#include <cmath>
#include <cstdint>
#include "core/Utils.hpp"
#include "core/Vec3.hpp"

namespace rt {

using Color = Vec3;

// Borne chaque composante dans [0,1]. Le gamma reste dans Image.
inline Color clampColor(const Color& c) noexcept {
    return Color{clamp(c.x(), 0.0, 1.0), clamp(c.y(), 0.0, 1.0), clamp(c.z(), 0.0, 1.0)};
}

// Convertit une couleur en trois octets (arrondi au plus proche). Par sécurité
// chaque composante est rebornée dans [0,1] avant conversion.
inline std::array<std::uint8_t, 3> toRgb8(const Color& c) noexcept {
    auto toByte = [](double v) {
        return static_cast<std::uint8_t>(std::lround(clamp(v, 0.0, 1.0) * 255.0));
    };
    return {toByte(c.x()), toByte(c.y()), toByte(c.z())};
}

} // namespace rt
