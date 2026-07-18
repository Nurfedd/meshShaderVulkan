#include "task_manager.hpp"
#include "task.h"
#include <unordered_set>
#include <chrono>
using namespace mt;

namespace nino_engine {
	TaskManager::TaskManager() {
		// base 16
        unsigned int coreNumbers = std::thread::hardware_concurrency();
        unsigned int coreBase = 16;

        uint32_t renderThreadCount = 1;
        uint32_t renderSubmitThreadCount = 2;
        uint32_t backgroundThreadCount = 1;
        uint32_t hiPriWorkerThreadCount = 8;
        uint32_t lowPriWorkerThreadCount = 4;
        
        if (coreNumbers > coreBase) {
            unsigned int coreDiff = coreNumbers - coreBase;
            hiPriWorkerThreadCount += (uint32_t)std::ceil(coreDiff / float(2));
            lowPriWorkerThreadCount += coreDiff / 2;
        }
        AddNewWorkerPool(Render_Task, renderThreadCount);
        AddNewWorkerPool(Render_Submit, renderSubmitThreadCount);
        AddNewWorkerPool(Background_Task, backgroundThreadCount);
        AddNewWorkerPool(HiPriWorker, hiPriWorkerThreadCount);
        AddNewWorkerPool(LowPriWorker, lowPriWorkerThreadCount);
	}

    WorkerPool* TaskManager::AddNewWorkerPool(TaskType taskType, uint32_t workerCount) {
        std::shared_ptr<WorkerPool> newWorkerPool = std::make_shared<WorkerPool>();
        WorkerPool* rawPtr = newWorkerPool.get();
        newWorkerPool->Start(workerCount);
        //newWorkerPool->taskQueue.onTaskCompleted.Add(this, &TaskManager::OnTaskCompleted);
        workerPools[taskType] = std::move(newWorkerPool);
        return rawPtr;
    }

	TaskManager::~TaskManager() {
        for (auto& pair : workerPools) {
            pair.second->Stop();
        }
	}

    std::shared_ptr< mt::Task> TaskManager::AsyncTask(TaskType taskType, std::function<void()> taskFunction) {
        return CreateNewTaskAndSubmit<Task>(taskType, nullptr, taskFunction);
    }

    std::shared_ptr< mt::Task> TaskManager::TaskMainThread(std::function<void()> taskFunction) {
        std::shared_ptr< mt::Task> task = CreateNewTask<Task>(nullptr, taskFunction);
        SubmitToMainThread(task.get());
        return task;
    }

    void TaskManager::TaskDelayedSeconds(TaskType taskType, Task* task, float seconds) {
        std::chrono::steady_clock::time_point beginChrono = std::chrono::high_resolution_clock::now();
        AsyncTask(TaskType::Background_Task, [this, taskType, task, seconds, beginChrono]() {
            TryStartTaskDelayedWithConditionSeconds(taskType, task, seconds, [this, task]() {
                std::lock_guard lock(currentTaskMutex);
                return std::ranges::find_if(currentTasks,[task](std::shared_ptr<Task>& otherTask) {
                    return otherTask.get() == task;
                    }) != currentTasks.end();
                }, beginChrono
            );

        });
    }
    std::shared_ptr< mt::Task> TaskManager::TaskDelayedSeconds(TaskType taskType, std::function<void()> taskFunction, float seconds) {
        std::shared_ptr< mt::Task> task = CreateNewTask<Task>(nullptr, taskFunction);
        TaskDelayedSeconds(taskType, task.get(), seconds);
        return task;
    }
    void TaskManager::CancelTask(Task* cancelTask) {
        std::lock_guard lock(currentTaskMutex);
        std::ranges::remove_if(currentTasks, [cancelTask](std::shared_ptr<Task>& task) {
            return cancelTask == task.get();
        });
    }
    void TaskManager::TryStartTaskDelayedSeconds(TaskType taskType, Task* task, float seconds, std::chrono::steady_clock::time_point beginChrono) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration_s = std::chrono::duration<float>(now - beginChrono).count();

        if (duration_s > seconds) {
            Submit(taskType, task);
        }
        else {
            // restart task
            AsyncTask(TaskType::Background_Task, [this, taskType, task, seconds, beginChrono]() {
                TryStartTaskDelayedSeconds(taskType, task, seconds, beginChrono);
            });
        }
    }

    void TaskManager::TryStartTaskDelayedWithConditionSeconds(TaskType taskType, Task* task, float seconds, std::function<bool()> condition, std::chrono::steady_clock::time_point beginChrono) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration_s = std::chrono::duration<float>(now - beginChrono).count();

        if (!condition())
            return;

        if (duration_s > seconds) {
            Submit(taskType, task);
        }
        else {
            // restart task
            AsyncTask(TaskType::Background_Task, [this, taskType, task, seconds,condition ,beginChrono]() {
                TryStartTaskDelayedWithConditionSeconds(taskType, task, seconds,condition, beginChrono);
                });
        }
    }
	void TaskManager::Submit(TaskType taskType,Task* task) {
        if (workerPools.contains(taskType)) {
            workerPools[taskType]->Submit(task);
        }
	}

    void TaskManager::SubmitToMainThread(mt::Task* task) {
        mainThreadTaskQueue.Push(task);
    }

    void TaskManager::UpdateMainThreadTasks()
    {
        mainThreadTaskQueue.ExecuteAll();
    }

    void TaskManager::WaitForTasks(Task** tasks, uint32_t taskNumber) {
        for (uint32_t i = 0; i < taskNumber; i++) {
            Task* task = tasks[i];
            task->Wait();
        }
    }

    void TaskManager::WaitForTasks(std::vector<Task*>& tasks) {
        WaitForTasks(tasks.data(), tasks.size());
    }
    std::vector<std::shared_ptr< mt::Task>> TaskManager::ParallelFor(uint32_t size, std::function<void(uint32_t index)> onExecuted) {
        if (onExecuted == nullptr)
            return std::vector<std::shared_ptr< mt::Task>>();

        std::vector<std::shared_ptr< mt::Task>> parallelTasks;
        uint32_t hiPriWorkerCount = workerPools[HiPriWorker]->GetWorkerCount();
        parallelTasks.reserve(hiPriWorkerCount);
        
        // just one per worker no need to dispatch
        if (hiPriWorkerCount >= size) {
            for (uint32_t i = 0; i < size; i++) {
                std::shared_ptr< mt::Task> newTask = CreateNewTaskAndSubmit<Task>(HiPriWorker, nullptr, [onExecuted, i]() {onExecuted(i); });
                parallelTasks.push_back(newTask);
            }
        }
        else {
            uint32_t workPerWorker = size / hiPriWorkerCount;
            uint32_t additionalForLastWorker = size % hiPriWorkerCount;

            for (uint32_t i = 0; i < hiPriWorkerCount; i++) {
                uint32_t workOffset = workPerWorker * i;
                uint32_t workSize = (i == hiPriWorkerCount - 1) ? workPerWorker + additionalForLastWorker : workPerWorker;

                std::shared_ptr< mt::Task> newTask = CreateNewTaskAndSubmit<Task>(HiPriWorker, nullptr, [onExecuted,workOffset, workSize]() {
                    
                    for (uint32_t j = workOffset; j < workOffset + workSize; j++) {
                        onExecuted(j);
                    }
                });
                parallelTasks.push_back(newTask);
            }
        }
        return parallelTasks;
    }
    void TaskManager::ParallelForWait(uint32_t size, std::function<void(uint32_t index)> onExecuted) {
        std::vector<std::shared_ptr< mt::Task>> parallelTasks = ParallelFor(size, onExecuted);
        for (std::shared_ptr< mt::Task>& task : parallelTasks) {
            task->Wait();
        }
    }

    void TaskManager::OnTaskCompleted(Task* completedTask) {
        std::lock_guard lock(currentTaskMutex);
        auto [first, last] = std::ranges::remove_if(currentTasks,
            [completedTask](std::shared_ptr<Task>& task) {
                return completedTask == task.get();
            });
        currentTasks.erase(first, last);
    }
}