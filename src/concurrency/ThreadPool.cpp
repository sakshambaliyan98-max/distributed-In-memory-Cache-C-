#include "concurrency/ThreadPool.h"
using namespace std;
namespace cache
{

ThreadPool::ThreadPool(size_t numThreads)
    : stopping_(false)
{
    if(numThreads == 0)
    {
        numThreads = 1;
    }

    workers_.reserve(numThreads);

    for(size_t i = 0; i < numThreads; ++i)
    {
        workers_.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool()
{
    shutdown();
}

void ThreadPool::workerLoop()
{
    for(;;)
    {
        function<void()> task;

        {
            unique_lock<mutex> lock(queueMutex_);

            condition_.wait(
                lock,
                [this]
                {
                    return stopping_.load() || !tasks_.empty();
                }
            );

            if(stopping_.load() && tasks_.empty())
            {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        // Run the task outside the lock so other workers/producers
        // aren't blocked while this one is executing.
        try
        {
            task();
        }
        catch(...)
        {
            // A single client/task must never take down a worker thread.
            // Swallow and keep the worker alive to pick up further work.
        }
    }
}

void ThreadPool::enqueue(function<void()> task)
{
    {
        lock_guard<mutex> lock(queueMutex_);

        if(stopping_.load())
        {
            return;
        }

        tasks_.push(move(task));
    }

    condition_.notify_one();
}

void ThreadPool::shutdown()
{
    {
        lock_guard<mutex> lock(queueMutex_);

        if(stopping_.load())
        {
            return;
        }

        stopping_.store(true);
    }

    condition_.notify_all();

    for(auto& worker : workers_)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }
}

size_t ThreadPool::workerCount() const
{
    return workers_.size();
}

size_t ThreadPool::pendingTasks() const
{
    lock_guard<mutex> lock(queueMutex_);

    return tasks_.size();
}

}
