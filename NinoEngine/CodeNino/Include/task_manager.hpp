#pragma once
#include "service.hpp"
#include "worker_pool.h"
#include "main_thread_queue.h"
#include "task.h"
#include <memory>
#include <vector>
#include <type_traits>
#include <functional>
#include <unordered_map>
namespace nino_engine {
	enum TaskType {
		Render_Task,
		Render_Submit,
		Background_Task,
		HiPriWorker,
		LowPriWorker
	};
	class TaskManager : public Service {
	public :
		TaskManager();
		~TaskManager();

		template<typename T>
		std::shared_ptr<T> CreateNewTask() {
			static_assert(std::is_base_of<mt::Task, T>::value);
			std::shared_ptr<T> newTask = std::make_shared<T>();
			{
				std::lock_guard lock(currentTaskMutex);
				currentTasks.push_back(newTask);
			}
			return newTask;
		}
		template<typename T>
		std::shared_ptr<T> CreateNewTask(std::function<void()> onCompleted, std::function<void()> onExecuted) {
			static_assert(std::is_base_of<mt::Task, T>::value);
			std::shared_ptr<T> newTask = std::make_shared<T>(onCompleted, onExecuted);
			
			{
				std::lock_guard lock(currentTaskMutex);
				currentTasks.push_back(newTask);
			}
			return newTask;
		}
		// usefull for task that don't need to be waited or synchronized
		std::shared_ptr<mt::Task> AsyncTask(TaskType taskType, std::function<void()> taskFunction);
		std::shared_ptr<mt::Task> TaskMainThread(std::function<void()> taskFunction);

		void TaskDelayedSeconds(TaskType taskType, mt::Task* task, float seconds);
		std::shared_ptr< mt::Task> TaskDelayedSeconds(TaskType taskType, std::function<void()> taskFunction, float seconds);
		// cancel and destroy a task works with TaskDelayedSeconds()
		void CancelTask(mt::Task* task);

		void Submit(TaskType taskType,mt::Task* task);
		void SubmitToMainThread(mt::Task* Task);

		void WaitForTasks(mt::Task** tasks, uint32_t taskNumber);
		void WaitForTasks(std::vector<mt::Task*>& tasks);
		template<typename T>
		std::shared_ptr<T> CreateNewTaskAndSubmit(TaskType taskType) {
			std::shared_ptr<T> newTask = CreateNewTask<T>();
			Submit(taskType,newTask);
			return newTask;
		}
		template<typename T>
		std::shared_ptr<T> CreateNewTaskAndSubmit(TaskType taskType,std::function<void()> onCompleted,std::function<void()> onExecuted) {
			std::shared_ptr<T> newTask = CreateNewTask<T>(onCompleted,onExecuted);
			Submit(taskType,newTask.get());
			return newTask;
		}
		std::vector<std::shared_ptr< mt::Task>> ParallelFor(uint32_t size, std::function<void(uint32_t index)> onExecuted);
		void ParallelForWait(uint32_t size, std::function<void(uint32_t index)> onExecuted);
		void UpdateMainThreadTasks();

	private :

		std::unordered_map<TaskType, std::shared_ptr<mt::WorkerPool>> workerPools;
		mt::MainThreadQueue mainThreadTaskQueue;
		std::vector<std::shared_ptr<mt::Task>> currentTasks;
		
		std::mutex currentTaskMutex;

		void TryStartTaskDelayedSeconds(TaskType taskType,mt::Task* task,float seconds, std::chrono::steady_clock::time_point beginChrono);
		void TryStartTaskDelayedWithConditionSeconds(TaskType taskType, mt::Task* task, float seconds,std::function<bool()> condition, std::chrono::steady_clock::time_point beginChrono);
		void OnTaskCompleted(mt::Task* task);
		mt::WorkerPool* AddNewWorkerPool(TaskType taskType, uint32_t workerCount);
	};
}