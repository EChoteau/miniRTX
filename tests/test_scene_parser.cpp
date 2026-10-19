// Fichier : test_scene_parser.cpp
// Rôle : squelette des tests du format texte de scène.
// Responsable : Personne C
// Dépendances : SceneParser.hpp, GoogleTest
// TODO : TODO(C): tester scène valide, directive inconnue et erreur de ligne.
// Cas : scène valide, lignes vides/commentaires, directive inconnue line 3,
// matériau absent, mauvaise arité, nan/inf et fichier introuvable.

#include <gtest/gtest.h>
#include "scene/SceneParser.hpp"

TEST(SceneParserTest, Placeholder) {
    EXPECT_TRUE(true);
}
