#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;
namespace cache
{

// A fixed-size pool of worker threads pulling from a shared task queue
// (classic producer-consumer). Producers call enqueue(); each worker
// blocks on the condition variable until a task is available or the pool shuts down

class ThreadPool
{
private:

    vector<thread> workers_;

    queue<function<void()>> tasks_;

    mutable mutex queueMutex_;

    condition_variable condition_;

    atomic<bool> stopping_;

    void workerLoop();

public:

    explicit ThreadPool(
        size_t numThreads = thread::hardware_concurrency()
    );

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Adds a task to the queue. Safe to call from any thread.
    // No-op (task is dropped) if the pool has already been shut down.
    void enqueue(function<void()> task);

    // Stops accepting new work and joins all worker threads.
    // Tasks already queued at the time of the call are still executed
    // before the corresponding worker exits. Safe to call multiple times.
    void shutdown();

    size_t workerCount() const;

    size_t pendingTasks() const;
};

}
