// Fichier : benchmark.cpp
// Rôle : squelette de mesure 1/2/4/8 threads et calcul du speedup.
// Responsable : Personne C
// Dépendances : Renderer.hpp, SceneParser.hpp, <chrono>, <iostream>
// EN CLAIR : ce programme compare le temps de rendu avec plusieurs nombres de threads.
// ENTRÉES / SORTIES : il reçoit demo.scene et affiche médianes et speedups.
// APPELÉ PAR : CMake ou l'étudiant C pendant le benchmark.
// PAS À FAIRE ICI : ne pas modifier le moteur de rendu ni les pixels.
// TODO(C) : charger la scène via RT_SOURCE_DIR, échauffer puis mesurer trois fois,
// vérifier l'identité des images et noter hardware_concurrency().

#include <chrono>
#include <iostream>
#include "render/Renderer.hpp"
#include "scene/SceneParser.hpp"

int main() {
    std::cout << "threads\ttime_ms\tspeedup\n";
    // TODO(C): remplacer cette présentation par échauffement + médiane de trois mesures.
    // TODO(C): afficher speedup = t1 / tN et comparer les images pour 1/2/4/8 threads.
    return 0;
}
