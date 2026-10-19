#pragma once

// Fichier : Vec3.hpp
// Rôle : vecteur 3D et opérations algébriques utilisées par tous les rayons.
// Responsable : Personne A
// Dépendances : <cmath>, <cstddef>, <ostream>, Utils.hpp
// TODO : TODO(A): compléter les tests de cas dégénérés et de division par zéro.

#include <cmath>
#include <cstddef>
#include <ostream>
#include "core/Utils.hpp"

namespace rt {

class Vec3 {
public:
    // EN CLAIR : Vec3 est la brique commune aux positions, directions, normales
    // et couleurs. Les opérations doivent rester simples et sans effet caché.
    // ENTRÉES / SORTIES : trois composantes doubles ; les méthodes renvoient un
    // vecteur, une longueur ou un produit scalaire.
    // APPELÉ PAR : presque toutes les classes du moteur.
    // PAS À FAIRE ICI : ne pas normaliser automatiquement les directions d'un Ray.
    // TODO(A): définir les règles de calcul et les cas dégénérés de chaque opération.
    constexpr Vec3() noexcept : x_(0.0), y_(0.0), z_(0.0) {}
    constexpr Vec3(double x, double y, double z) noexcept : x_(x), y_(y), z_(z) {}

    // TODO(A): exposer x_, y_ et z_ sans les modifier.
    double x() const noexcept { return 0.0; }
    double y() const noexcept { return 0.0; }
    double z() const noexcept { return 0.0; }
    // TODO(A): fournir l'accès mutable utilisé par les constructions de couleurs.
    // TODO(A): retourner une référence mutable vers x_.
    double& x() noexcept { return x_; }
    // TODO(A): retourner une référence mutable vers y_.
    double& y() noexcept { return y_; }
    // TODO(A): retourner une référence mutable vers z_.
    double& z() noexcept { return z_; }

    // TODO(A): gérer l'indexation 0=x, 1=y, 2=z et ses limites.
    double operator[](std::size_t) const noexcept { return 0.0; }
    // TODO(A): retourner la composante mutable sélectionnée par l'index.
    double& operator[](std::size_t) noexcept { return x_; }
    // TODO(A): implémenter les opérations composante par composante.
    Vec3 operator-() const noexcept { return Vec3{}; }
    Vec3& operator+=(const Vec3&) noexcept { return *this; }
    Vec3& operator-=(const Vec3&) noexcept { return *this; }
    Vec3& operator*=(double) noexcept { return *this; }
    Vec3& operator/=(double) noexcept { return *this; }
    Vec3& operator*=(const Vec3&) noexcept { return *this; }
    Vec3& operator/=(const Vec3&) noexcept { return *this; }
    // TODO(A): calculer longueur carrée, longueur et normalisation.
    double lengthSquared() const noexcept { return 0.0; }
    double length() const noexcept { return 0.0; }
    Vec3 normalized() const noexcept { return Vec3{}; }
    // TODO(A): calculer dot, cross et reflect ; normalized() retourne le nul
    // si la longueur <= EPSILON et reflect utilise les composantes directement.
    static double dot(const Vec3&, const Vec3&) noexcept { return 0.0; }
    static Vec3 cross(const Vec3&, const Vec3&) noexcept { return Vec3{}; }
    static Vec3 reflect(const Vec3&, const Vec3&) noexcept { return Vec3{}; }

private:
    double x_;
    double y_;
    double z_;
};

// TODO(A): déléguer ces opérateurs libres aux opérations de Vec3.
inline Vec3 operator+(Vec3, const Vec3&) noexcept { return Vec3{}; }
inline Vec3 operator-(Vec3, const Vec3&) noexcept { return Vec3{}; }
inline Vec3 operator*(Vec3, double) noexcept { return Vec3{}; }
inline Vec3 operator*(double, Vec3) noexcept { return Vec3{}; }
inline Vec3 operator/(Vec3, double) noexcept { return Vec3{}; }
inline Vec3 operator*(Vec3, const Vec3&) noexcept { return Vec3{}; }
inline Vec3 operator/(Vec3, const Vec3&) noexcept { return Vec3{}; }
inline double dot(const Vec3&, const Vec3&) noexcept { return 0.0; }
inline Vec3 cross(const Vec3&, const Vec3&) noexcept { return Vec3{}; }
inline Vec3 reflect(const Vec3&, const Vec3&) noexcept { return Vec3{}; }
inline std::ostream& operator<<(std::ostream& stream, const Vec3& value) {
    // TODO(A): écrire les trois composantes séparées par des espaces.
    (void)value;
    return stream;
}

} // namespace rt
