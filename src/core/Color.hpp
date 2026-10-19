#pragma once

// Fichier : Color.hpp
// Rôle : couleur RGB flottante, clamp et conversion PPM 8 bits.
// Responsable : Personne A
// Dépendances : <array>, <cstdint>, Vec3.hpp
// TODO : TODO(A): décider si la conversion gamma doit rester ici ou dans Image.

#include <array>
#include <cstdint>
#include "core/Vec3.hpp"

namespace rt {

using Color = Vec3;

// Borne chaque composante RGB, puis applique éventuellement gamma dans Image.
inline Color clampColor(const Color& color) noexcept {
    return Color(clamp(color.x(), 0.0, 1.0),
                 clamp(color.y(), 0.0, 1.0),
                 clamp(color.z(), 0.0, 1.0));
}

inline std::array<std::uint8_t, 3> toRgb8(const Color& color) noexcept {
    const Color bounded = clampColor(color);
    return {static_cast<std::uint8_t>(255.999 * bounded.x()),
            static_cast<std::uint8_t>(255.999 * bounded.y()),
            static_cast<std::uint8_t>(255.999 * bounded.z())};
}

} // namespace rt
