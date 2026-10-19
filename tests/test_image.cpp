// Fichier : test_image.cpp
// Rôle : squelette des tests du buffer et de l'export PPM.
// Responsable : Personne C
// Dépendances : Image.hpp, GoogleTest
// TODO : TODO(C): vérifier dimensions, pixels, gamma et en-tête P6.

#include <gtest/gtest.h>
#include "render/Image.hpp"

TEST(ImageTest, Placeholder) {
    rt::Image image(2, 2);
    image.setPixel(0, 0, rt::Color(1.0, 0.0, 0.0));
    EXPECT_DOUBLE_EQ(image.pixel(0, 0).x(), 1.0);
}
