// Fichier : test_vec3.cpp
// Rôle : squelette des tests de l'algèbre vectorielle.
// Responsable : Personne A
// Dépendances : Vec3.hpp, GoogleTest
// TODO : TODO(A): ajouter assertions sur dot, cross, normalisation et indexation.
// Cas : dot((1,2,3),(4,5,6))=32 ; cross((1,0,0),(0,1,0))=(0,0,1) ;
// length((3,4,0))=5 ; normalized(0,0,0)=(0,0,0) ; reflect=(1,1,0).

#include <gtest/gtest.h>
#include "core/Vec3.hpp"

TEST(Vec3Test, Placeholder) {
    EXPECT_DOUBLE_EQ(rt::Vec3(1.0, 2.0, 3.0).x(), 1.0);
}
