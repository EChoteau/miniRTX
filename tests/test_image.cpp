// Fichier : test_image.cpp
// Rôle : squelette des tests du buffer et de l'export PPM.
// Responsable : Personne C
// Dépendances : Image.hpp, GoogleTest
// TODO : TODO(C): vérifier dimensions, pixels, gamma et en-tête P6.

#include <gtest/gtest.h>
#include "render/Image.hpp"

TEST(ImageTest, Placeholder) {
    // Doit échouer tant que la Personne C n'a pas implémenté setPixel/pixel.
    rt::Image image(2, 2);
    image.setPixel(0, 0, rt::Color(1.0, 0.0, 0.0));
    EXPECT_TRUE(true);
}
