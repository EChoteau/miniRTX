// Fichier : test_material.cpp
// Rôle : squelette des tests des paramètres de matériau et de Phong.
// Responsable : Personne B
// Dépendances : Material.hpp, GoogleTest
// TODO : TODO(B): tester contribution ambiante, diffuse, spéculaire et réflexion.
// Cas : résultat ambiant/diffus (0.9,0,0), lumière sous la surface ambiant seul,
// spéculaire (1.4,0.5,0.5), objets d'ombre devant/derrière/sous la lampe.

#include <gtest/gtest.h>
#include "material/Material.hpp"

TEST(MaterialTest, Placeholder) {
    EXPECT_DOUBLE_EQ(rt::Material{}.reflectivity, 0.0);
}
