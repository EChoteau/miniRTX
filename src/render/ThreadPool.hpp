#pragma once

// Fichier : ThreadPool.hpp
// Rôle : abstraction minimale de tâches parallèles pour les tuiles.
// Responsable : Personne C
// Dépendances : <functional>, <thread>, <vector>
// TODO : TODO(C): remplacer par un pool persistant si le benchmark le justifie.

#include <functional>
#include <thread>
#include <vector>

namespace rt {

class ThreadPool {
public:
    // EN CLAIR : ThreadPool lance un groupe de workers qui exécutent une même
    // boucle de travail, comme plusieurs étudiants prenant des tuiles.
    // ENTRÉES / SORTIES : il reçoit un nombre de workers et une tâche, sans
    // produire lui-même de pixels.
    // APPELÉ PAR : Renderer::render.
    // PAS À FAIRE ICI : ne pas partager le RNG entre les workers.
    // TODO(C): documenter la capture et la relance des exceptions après join.
    // Lance workerCount threads, chacun exécutant la même boucle de tâches.
    // TODO(C): mémoriser au moins un worker même si workerCount vaut zéro.
    explicit ThreadPool(unsigned workerCount);
    // TODO(C): attendre/arrêter les workers si le modèle évolue vers un pool persistant.
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    void run(const std::function<void()>& task);

private:
    unsigned workerCount_;
};

} // namespace rt
