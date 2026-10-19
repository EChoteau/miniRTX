#pragma once

// Fichier : SceneParser.hpp
// Rôle : lecture du format texte de scène et diagnostics avec numéro de ligne.
// Responsable : Personne C
// Dépendances : Scene.hpp, <stdexcept>, <string>
// TODO : TODO(C): valider arité, nombres finis, matériaux référencés et directives inconnues.

#include <stdexcept>
#include <string>
#include "scene/Scene.hpp"

namespace rt {

class SceneParser {
public:
    // EN CLAIR : le parseur transforme chaque ligne .scene en objets de Scene.
    // ENTRÉES / SORTIES : il reçoit un chemin et renvoie une Scene ou une erreur
    // localisée par numéro de ligne.
    // APPELÉ PAR : main et le benchmark.
    // PAS À FAIRE ICI : ne pas rendre l'image ni appliquer l'éclairage.
    // TODO(C): faire suivre les données du texte vers les constructeurs.
    // Parse camera, background, material, light, sphere, plane et triangle optionnel.
    // Lève std::runtime_error("line N: ...") au premier problème.
    static Scene parseFile(const std::string& path);
};

} // namespace rt
