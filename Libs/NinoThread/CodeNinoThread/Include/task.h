#pragma once
#include <functional>
#include <condition_variable>
namespace mt
{
    class Worker;
    class Task
    {
    public:
        Task() = default;
        Task(std::function<void()> onCompleted, std::function<void()> onExecuted);
        virtual ~Task() = default;

        void Wait();
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        virtual void Execute();
        virtual void OnComplete();

    private:
        std::condition_variable condition_variable;
        std::function<void()> onCompleted;
        std::function<void()> onExecuted;
        std::mutex mutex;
        bool completed = false;
    };
}