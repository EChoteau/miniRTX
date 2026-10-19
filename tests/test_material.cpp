// Fichier : test_material.cpp
// Rôle : squelette des tests des paramètres de matériau et de Phong.
// Responsable : Personne B
// Dépendances : Material.hpp, GoogleTest
// TODO : TODO(B): tester contribution ambiante, diffuse, spéculaire et réflexion.

#include <gtest/gtest.h>
#include "material/Material.hpp"

TEST(MaterialTest, Placeholder) {
    EXPECT_DOUBLE_EQ(rt::Material{}.reflectivity, 0.0);
}
