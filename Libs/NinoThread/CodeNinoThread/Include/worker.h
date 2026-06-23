#pragma once
#include <thread>
#include "task_queue.h"
#include "main_thread_queue.h"

namespace mt
{
    class Worker
    {
    public:
        Worker(TaskQueue& queue, MainThreadQueue& mainQueue);
        ~Worker();

        void Start();
        void Stop();

    private:
        void Run(std::stop_token stoken);

    private:
        TaskQueue& taskQueue;
        MainThreadQueue& mainQueue;
        std::jthread thread;
    };
}