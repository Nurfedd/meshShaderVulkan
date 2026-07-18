#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "delegate.hpp"
namespace mt
{
    class Task;
    DECLARE_DELEGATE(OnTaskCompleted, Task*)

    class TaskQueue
    {
    public:
        void Push(Task* task);
        Task* Pop();
        void Stop();
        OnTaskCompleted onTaskCompleted;
    private:
        std::queue<Task*> queue;
        std::mutex mutex;
        std::condition_variable cv;
        bool stopped = false;
    };
}