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
        virtual ~Task() = default;

        void Wait();
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;

        virtual void Execute() = 0;
        virtual void OnComplete() {};

    private:
        void NotifyExecuted();
        std::condition_variable condition_variable;
        std::mutex mutex;
        bool completed = false;
        friend class Worker;
    };
}