// Fichier : ThreadPool.cpp
// Rôle : squelette d'exécution concurrente d'une tâche.
// Responsable : Personne C
// Dépendances : ThreadPool.hpp
// TODO : TODO(C): introduire une file de tâches et synchroniser proprement les workers.
// CHAÎNE : Renderer fabrique une tâche de tuiles -> ThreadPool la distribue ->
// chaque worker remonte ses exceptions après join.

#include "render/ThreadPool.hpp"

namespace rt {

ThreadPool::ThreadPool(unsigned workerCount) : workerCount_(workerCount) {}
ThreadPool::~ThreadPool() = default;

void ThreadPool::run(const std::function<void()>& task) {
    // TODO(C): capturer std::exception_ptr dans chaque worker et la relancer
    // après join() ; workerCount == 0 est remplacé par 1. Renderer passera
    // une lambda qui boucle sur std::atomic<int> nextTile avec fetch_add(1).
    (void)task;
    (void)workerCount_;
}

} // namespace rt
