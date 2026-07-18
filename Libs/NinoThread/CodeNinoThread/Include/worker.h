#pragma once
#include <thread>
#include "task_queue.h"

namespace mt
{
    class Worker
    {
    public:
        Worker(TaskQueue& queue);
        ~Worker();

        void Start();
        void Stop();

    private:
        void Run(std::stop_token stoken);

    private:
        TaskQueue& taskQueue;
        std::jthread thread;
    };
}