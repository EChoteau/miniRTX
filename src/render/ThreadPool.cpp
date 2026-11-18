// Fichier : ThreadPool.cpp
// Rôle : exécution concurrente d'une tâche sur plusieurs workers.
// Responsable : Personne C
// Dépendances : ThreadPool.hpp
// TODO : TODO(C): introduire une file de tâches et synchroniser proprement les workers.
// CHAÎNE : Renderer fabrique une tâche de tuiles -> ThreadPool la distribue ->
// chaque worker remonte ses exceptions après join.

#include "render/ThreadPool.hpp"

#include <algorithm>
#include <exception>
#include <mutex>

namespace rt {

// workerCount == 0 est remplacé par 1.
ThreadPool::ThreadPool(unsigned workerCount)
    : workerCount_(std::max(1u, workerCount)) {}

ThreadPool::~ThreadPool() = default;

void ThreadPool::run(const std::function<void()>& task) {
    std::exception_ptr firstError;
    std::mutex errorMutex;

    // Chaque worker exécute la tâche et mémorise la première exception levée.
    auto guarded = [&]() {
        try {
            task();
        } catch (...) {
            std::lock_guard<std::mutex> lock(errorMutex);
            if (!firstError) firstError = std::current_exception();
        }
    };

    // workerCount_ - 1 threads supplémentaires ; le thread appelant est le
    // dernier worker. Avec 1 worker, aucun thread n'est créé.
    std::vector<std::thread> threads;
    threads.reserve(workerCount_ - 1);
    for (unsigned i = 0; i + 1 < workerCount_; ++i)
        threads.emplace_back(guarded);

    guarded();

    for (auto& t : threads) t.join();

    // Relance seulement après tous les join().
    if (firstError) std::rethrow_exception(firstError);
}

} // namespace rt