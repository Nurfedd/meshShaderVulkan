#include "main_thread_queue.h"
#include "task.h"

namespace mt
{
    void MainThreadQueue::Push(Task* task)
    {
        std::lock_guard lock(mutex);
        mainQueue.push(task);
    }

    std::vector<Task*> MainThreadQueue::ExecuteAll()
    {
        std::queue<Task*> local;
        std::vector<Task*> executedTask;
        {
            std::lock_guard lock(mutex);
            std::swap(local, mainQueue);
        }

        while (!local.empty())
        {
            Task* task = local.front();
            local.pop();
            task->OnComplete();
            executedTask.push_back(task);
        }
        return executedTask;
    }
}