// Fichier : ThreadPool.cpp
// Rôle : squelette d'exécution concurrente d'une tâche.
// Responsable : Personne C
// Dépendances : ThreadPool.hpp
// TODO : TODO(C): introduire une file de tâches et synchroniser proprement les workers.

#include "render/ThreadPool.hpp"

namespace rt {

ThreadPool::ThreadPool(unsigned workerCount) : workerCount_(workerCount) {}
ThreadPool::~ThreadPool() = default;

void ThreadPool::run(const std::function<void()>& task) {
    // TODO(C): répartir les tuiles sans partager de RNG entre threads.
    std::vector<std::thread> workers;
    workers.reserve(workerCount_);
    for (unsigned index = 0; index < workerCount_; ++index) {
        workers.emplace_back(task);
    }
    for (std::thread& worker : workers) {
        worker.join();
    }
}

} // namespace rt
