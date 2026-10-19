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
    // Parse camera, background, material, light, sphere, plane et triangle optionnel.
    // Lève std::runtime_error("line N: ...") au premier problème.
    Scene parseFile(const std::string& path);
};

} // namespace rt
