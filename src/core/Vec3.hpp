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
    // Construit le vecteur nul, ou le vecteur (x, y, z).
    constexpr Vec3() noexcept : x_(0.0), y_(0.0), z_(0.0) {}
    constexpr Vec3(double x, double y, double z) noexcept : x_(x), y_(y), z_(z) {}

    double x() const noexcept { return x_; }
    double y() const noexcept { return y_; }
    double z() const noexcept { return z_; }
    double& x() noexcept { return x_; }
    double& y() noexcept { return y_; }
    double& z() noexcept { return z_; }

    double operator[](std::size_t index) const noexcept {
        return index == 0 ? x_ : (index == 1 ? y_ : z_);
    }
    double& operator[](std::size_t index) noexcept {
        return index == 0 ? x_ : (index == 1 ? y_ : z_);
    }

    Vec3 operator-() const noexcept { return Vec3(-x_, -y_, -z_); }
    Vec3& operator+=(const Vec3& other) noexcept { x_ += other.x_; y_ += other.y_; z_ += other.z_; return *this; }
    Vec3& operator-=(const Vec3& other) noexcept { x_ -= other.x_; y_ -= other.y_; z_ -= other.z_; return *this; }
    Vec3& operator*=(double scalar) noexcept { x_ *= scalar; y_ *= scalar; z_ *= scalar; return *this; }
    Vec3& operator/=(double scalar) noexcept { x_ /= scalar; y_ /= scalar; z_ /= scalar; return *this; }
    Vec3& operator*=(const Vec3& other) noexcept { x_ *= other.x_; y_ *= other.y_; z_ *= other.z_; return *this; }
    Vec3& operator/=(const Vec3& other) noexcept { x_ /= other.x_; y_ /= other.y_; z_ /= other.z_; return *this; }

    double lengthSquared() const noexcept { return dot(*this, *this); }
    double length() const noexcept { return std::sqrt(lengthSquared()); }
    Vec3 normalized() const noexcept {
        const double len = length();
        return len <= EPSILON ? Vec3{} : *this / len;
    }

    static double dot(const Vec3& lhs, const Vec3& rhs) noexcept {
        return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
    }
    static Vec3 cross(const Vec3& lhs, const Vec3& rhs) noexcept {
        return Vec3(lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
                    lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
                    lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_);
    }
    static Vec3 reflect(const Vec3& vector, const Vec3& normal) noexcept {
        return vector - 2.0 * dot(vector, normal) * normal;
    }

private:
    double x_;
    double y_;
    double z_;
};

inline Vec3 operator+(Vec3 lhs, const Vec3& rhs) noexcept { return lhs += rhs; }
inline Vec3 operator-(Vec3 lhs, const Vec3& rhs) noexcept { return lhs -= rhs; }
inline Vec3 operator*(Vec3 lhs, double scalar) noexcept { return lhs *= scalar; }
inline Vec3 operator*(double scalar, Vec3 rhs) noexcept { return rhs *= scalar; }
inline Vec3 operator/(Vec3 lhs, double scalar) noexcept { return lhs /= scalar; }
inline Vec3 operator*(Vec3 lhs, const Vec3& rhs) noexcept { return lhs *= rhs; }
inline Vec3 operator/(Vec3 lhs, const Vec3& rhs) noexcept { return lhs /= rhs; }
inline double dot(const Vec3& lhs, const Vec3& rhs) noexcept { return Vec3::dot(lhs, rhs); }
inline Vec3 cross(const Vec3& lhs, const Vec3& rhs) noexcept { return Vec3::cross(lhs, rhs); }
inline Vec3 reflect(const Vec3& vector, const Vec3& normal) noexcept { return Vec3::reflect(vector, normal); }
inline std::ostream& operator<<(std::ostream& stream, const Vec3& value) {
    return stream << value.x() << ' ' << value.y() << ' ' << value.z();
}

} // namespace rt
