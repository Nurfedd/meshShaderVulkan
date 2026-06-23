#include "task.h"
#include "worker.h"
namespace mt {
	void Task::Wait() {
		std::unique_lock lock(mutex);

		condition_variable.wait(lock, [this] {
			return completed;
			});
	}
	void Task::NotifyExecuted() {
		{
			std::lock_guard lock(mutex);
			completed = true;
		}

		condition_variable.notify_all();
	}
}