// Fichier : SceneParser.cpp
// Rôle : squelette du parseur ligne par ligne et diagnostics contextualisés.
// Responsable : Personne C
// Dépendances : SceneParser.hpp, <fstream>
// TODO : TODO(C): implémenter chaque directive et inclure le numéro de ligne dans les erreurs.

#include "scene/SceneParser.hpp"
#include <fstream>

namespace rt {

Scene SceneParser::parseFile(const std::string& path) {
    // TODO(C): ouvrir path, ignorer commentaires/lignes vides, dispatcher les directives.
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("cannot open scene file: " + path);
    }
    return Scene{};
}

} // namespace rt
