#pragma once
#include <queue>
#include <mutex>

namespace mt
{
    class Task;

    class MainThreadQueue
    {
    public:
        void Push(Task* task);
        std::vector<Task*> ExecuteAll();

    private:
        std::queue<Task*> mainQueue;
        std::mutex mutex;
    };
}