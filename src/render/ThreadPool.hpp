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
    // Lance workerCount threads, chacun exécutant la même boucle de tâches.
    explicit ThreadPool(unsigned workerCount);
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    void run(const std::function<void()>& task);

private:
    unsigned workerCount_;
};

} // namespace rt
