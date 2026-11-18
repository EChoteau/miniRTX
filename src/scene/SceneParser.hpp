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

/// Parse un fichier de scène et construit la scène correspondante.
class SceneParser {
public:
    /// @param path Chemin du fichier de scène à lire.
    /// @return Scène construite à partir du fichier.
    /// @throws std::runtime_error si le fichier ou son contenu est invalide.
    static Scene parseFile(const std::string& path);
};

} // namespace rt
