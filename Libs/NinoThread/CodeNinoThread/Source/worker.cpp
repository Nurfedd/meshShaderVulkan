#include "worker.h"
#include "task.h"
namespace mt
{
    Worker::Worker(TaskQueue& queue)
        : taskQueue(queue)
    {
    }

    Worker::~Worker()
    {
        Stop();
    }

    void Worker::Start()
    {
        thread = std::jthread(std::bind_front(&Worker::Run, this));
    }

    void Worker::Stop()
    {
        if (thread.joinable())
            thread.request_stop();
    }

    void Worker::Run(std::stop_token stoken)
    {
        while (!stoken.stop_requested())
        {
            Task* task = taskQueue.Pop();
            if (!task)
                break;

            task->Execute();
            task->OnComplete();
            taskQueue.onTaskCompleted.Broadcast(task);
        }
    }
}