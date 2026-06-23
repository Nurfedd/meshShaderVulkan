#include "task_queue.h"
#include "task.h"

namespace mt
{
    void TaskQueue::Push(Task* task)
    {
        {
            std::lock_guard lock(mutex);
            queue.push(std::move(task));
        }
        cv.notify_one();
    }

    Task* TaskQueue::Pop()
    {
        std::unique_lock lock(mutex);

        cv.wait(lock, [&]
        {
            return stopped || !queue.empty();
        });

        if (stopped && queue.empty())
            return nullptr;

        Task* task = queue.front();
        queue.pop();
        return task;
    }

    void TaskQueue::Stop()
    {
        {
            std::lock_guard lock(mutex);
            stopped = true;
        }
        cv.notify_all();
    }
}