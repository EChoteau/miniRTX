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

    double x() const noexcept { return x_; }
    double y() const noexcept { return y_; }
    double z() const noexcept { return z_; }

    double& x() noexcept { return x_; }    
    double& y() noexcept { return y_; }
    double& z() noexcept { return z_; }

    double operator[](std::size_t index) const noexcept { 
        switch (index) {
            case 0: return x_;
            case 1: return y_;
            default: return z_;
        }
     }
    double& operator[](std::size_t index) noexcept { 
        switch (index) {
            case 0: return x_;
            case 1: return y_;
            default: return z_;
        }
     }

    Vec3 operator-() const noexcept {
        return Vec3{-x_, -y_, -z_};
    }
    Vec3& operator+=(const Vec3& other) noexcept { 
        x_ += other.x_;
        y_ += other.y_;
        z_ += other.z_;
        return *this;
    }
    Vec3& operator-=(const Vec3& other) noexcept { 
        x_ -= other.x_;
        y_ -= other.y_;
        z_ -= other.z_;
        return *this; 
    }
    Vec3& operator*=(double other) noexcept { 
        x_ *= other;
        y_ *= other;
        z_ *= other;
        return *this;
    }
    Vec3& operator/=(double other) noexcept { 
        x_ /= other;
        y_ /= other;
        z_ /= other;
        return *this;
    }
    Vec3& operator*=(const Vec3& other) noexcept { 
        x_ *= other.x_;
        y_ *= other.y_;
        z_ *= other.z_;
        return *this;
    }
    Vec3& operator/=(const Vec3& other) noexcept { 
        x_ /= other.x_;
        y_ /= other.y_;
        z_ /= other.z_;
        return *this;
    }
    double lengthSquared() const noexcept {
        return x_ * x_ + y_ * y_ + z_ * z_;
     }
    double length() const noexcept {
        return std::sqrt(lengthSquared());
    }
    Vec3 normalized() const noexcept {
        const double length = this->length();
        return length <= EPSILON ? Vec3{} : *this / length; }
    static double dot(const Vec3& a, const Vec3& b) noexcept { return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_; }
    static Vec3 cross(const Vec3& a, const Vec3& b) noexcept { return Vec3{a.y_ * b.z_ - b.y_ * a.z_, a.z_ * b.x_ - b.z_ * a.x_, a.x_ * b.y_ - b.x_ * a.y_}; }
    static Vec3 reflect(const Vec3& incident, const Vec3& normal) noexcept { return incident - 2 * normal * dot(incident, normal); }

private:
    double x_;
    double y_;
    double z_;
};

inline Vec3 operator+(Vec3 a, const Vec3& b) noexcept { return a += b; }

inline Vec3 operator-(Vec3 a, const Vec3& b) noexcept { return a -= b; }
inline Vec3 operator*(Vec3 a, double b) noexcept { return a *= b; }
inline Vec3 operator*(double a, Vec3 b) noexcept { return b *= a; }
inline Vec3 operator/(Vec3 a, double b) noexcept { return a /= b; }
inline Vec3 operator*(Vec3 a, const Vec3& b) noexcept { return a *= b; }
inline Vec3 operator/(Vec3 a, const Vec3& b) noexcept { return a /= b; }
inline double dot(const Vec3& a, const Vec3& b) noexcept { return Vec3::dot(a, b); }
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept { return Vec3::cross(a, b); }
inline Vec3 reflect(const Vec3& incident, const Vec3& normal) noexcept { return Vec3::reflect(incident, normal); }
inline std::ostream& operator<<(std::ostream& stream, const Vec3& value) {
    stream << value.x() << ' ' << value.y() << ' ' << value.z();
    return stream;
}

} // namespace rt
