#include "task_manager.hpp"
#include "task.h"
#include <unordered_set>
using namespace mt;
namespace nino_engine {
	TaskManager::TaskManager() {
		workerPool.Start();
	}

	TaskManager::~TaskManager() {
		workerPool.Stop();
	}

	void TaskManager::Submit(Task* task) {
		workerPool.Submit(task);
	}

    void TaskManager::UpdateMainThreadTasks()
    {
        std::vector<Task*> removedTask = workerPool.UpdateMainThread();

        if (removedTask.empty()) {
            return;
        }

        std::unordered_set<Task*> taskToRemoveSet(
            removedTask.begin(),
            removedTask.end()
        );

        currentTasks.erase(
            std::remove_if(
                currentTasks.begin(),
                currentTasks.end(),
                [&](const std::unique_ptr<Task>& task)
                {
                    return taskToRemoveSet.contains(task.get());
                }
            ),
            currentTasks.end()
        );
    }
}