#pragma once
#include <vector>
#include <memory>

#include "worker.h"

namespace mt
{
    class WorkerPool
    {
    public:
        WorkerPool();
        ~WorkerPool();

        void Start(size_t threadCount = std::thread::hardware_concurrency());
        void Stop();

        void Submit(Task* task);
        std::vector<Task*> UpdateMainThread();

    private:
        TaskQueue taskQueue;
        MainThreadQueue mainQueue;
        std::vector<std::unique_ptr<Worker>> workers;
    };
}