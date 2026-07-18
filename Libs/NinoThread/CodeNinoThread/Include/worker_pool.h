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

        void Start(size_t threadCount);
        void Stop();
        void Submit(Task* task);
        uint32_t GetWorkerCount();
        TaskQueue taskQueue;
    private:
        
        std::vector<std::unique_ptr<Worker>> workers;
    };
}