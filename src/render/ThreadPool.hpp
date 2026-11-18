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
    // Capture des exceptions : chaque worker attrape toute exception levée par
    // la tâche et en mémorise la première (std::exception_ptr). Elle est
    // relancée par run() une fois TOUS les threads joints.
    // Un workerCount de 0 est remplacé par 1.
    explicit ThreadPool(unsigned workerCount);
    // TODO(C): attendre/arrêter les workers si le modèle évolue vers un pool persistant.
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    // Exécute la même tâche sur workerCount workers (le thread appelant en est
    // un) et bloque jusqu'à la fin de tous. Relance la première exception.
    void run(const std::function<void()>& task);

private:
    unsigned workerCount_;
};

} // namespace rt