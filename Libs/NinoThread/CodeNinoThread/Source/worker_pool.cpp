#include "worker_pool.h"
#include <thread>

namespace mt
{
    WorkerPool::WorkerPool() = default;

    WorkerPool::~WorkerPool()
    {
        Stop();
    }

    void WorkerPool::Start(size_t threadCount)
    {
        workers.reserve(threadCount);

        for (size_t i = 0; i < threadCount; ++i)
        {
            auto worker = std::make_unique<Worker>(taskQueue, mainQueue);
            worker->Start();
            workers.push_back(std::move(worker));
        }
    }

    void WorkerPool::Stop()
    {
        taskQueue.Stop();
        workers.clear();
    }

    void WorkerPool::Submit(Task* task)
    {
        taskQueue.Push(task);
    }

    std::vector<Task*> WorkerPool::UpdateMainThread()
    {
        return mainQueue.ExecuteAll();
    }
}