#pragma once
#include "service.hpp"
#include "worker_pool.h"
#include <memory>
#include <vector>
#include <type_traits>
namespace nino_engine {
	class TaskManager : public Service {
	public :
		TaskManager();
		~TaskManager();

		template<typename T>
		T* CreateNewTask() {
			static_assert(std::is_base_of<mt::Task, T>::value);
			std::unique_ptr<T> newTask = std::make_unique<T>();

			T* rawPtr = newTask.get();
			currentTasks.push_back(std::move(newTask));
			return rawPtr;
		}
		
		void Submit(mt::Task* task);

		template<typename T>
		T* CreateNewTaskAndSubmit() {
			T* newTask = CreateNewTask<T>();
			Submit(newTask);
			return newTask;
		}

		void UpdateMainThreadTasks();
	private :
		mt::WorkerPool workerPool;
		std::vector<std::unique_ptr<mt::Task>> currentTasks;
	};
}