// Fichier : SceneParser.cpp
// Rôle : squelette du parseur ligne par ligne et diagnostics contextualisés.
// Responsable : Personne C
// Dépendances : SceneParser.hpp, <fstream>
// TODO : TODO(C): implémenter chaque directive et inclure le numéro de ligne dans les erreurs.
// CHAÎNE : fichier texte -> tokens -> validations -> constructeurs -> Scene.
// Le flux inverse est Renderer -> Scene ; le parser ne rend jamais l'image.

#include "scene/SceneParser.hpp"
#include <fstream>

namespace rt {

Scene SceneParser::parseFile(const std::string&) {
    // TODO(C): ouvrir le fichier, remplir CameraParams (up = (0,1,0) par
    // défaut), ignorer commentaires/lignes vides et dispatcher les directives.
    // TODO(C): valider arité exacte, nombres finis, ordre des matériaux,
    // doublons, bornes des paramètres et directive camera unique obligatoire.
    // TODO(C): rattraper les invalid_argument des objets et les relancer avec
    // le préfixe « line N: » ; signaler toute directive inconnue de la même façon.
    return Scene{};
}

} // namespace rt
