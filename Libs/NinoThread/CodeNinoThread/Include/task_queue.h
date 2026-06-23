#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace mt
{
    class Task;

    class TaskQueue
    {
    public:
        void Push(Task* task);
        Task* Pop();
        void Stop();

    private:
        std::queue<Task*> queue;
        std::mutex mutex;
        std::condition_variable cv;
        bool stopped = false;
    };
}