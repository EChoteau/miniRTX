// Fichier : benchmark.cpp
// Rôle : squelette de mesure 1/2/4/8 threads et calcul du speedup.
// Responsable : Personne C
// Dépendances : Renderer.hpp, SceneParser.hpp, <chrono>, <iostream>
// TODO : TODO(C): charger demo.scene, rendre chaque configuration et afficher le tableau.

#include <chrono>
#include <iostream>
#include "render/Renderer.hpp"
#include "scene/SceneParser.hpp"

int main() {
    std::cout << "threads\ttime_ms\tspeedup\n";
    for (unsigned threads : {1U, 2U, 4U, 8U}) {
        const auto start = std::chrono::steady_clock::now();
        // TODO(C): parser la scène et appeler Renderer::render avec threads.
        const auto stop = std::chrono::steady_clock::now();
        const auto milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << threads << '\t' << milliseconds << "\tTODO\n";
    }
    return 0;
}
