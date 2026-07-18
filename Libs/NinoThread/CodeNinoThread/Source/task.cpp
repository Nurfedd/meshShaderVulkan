#include "task.h"
#include "worker.h"
namespace mt {
	Task::Task(std::function<void()> _onCompleted,std::function<void()> _onExecuted) {
		onCompleted = _onCompleted;
		onExecuted = _onExecuted;
	}
	void Task::Wait() {
		std::unique_lock lock(mutex);

		condition_variable.wait(lock, [this] {
			return completed;
			});
	}
	
	void Task::OnComplete() {
		{
			std::lock_guard lock(mutex);
			completed = true;
		}

		condition_variable.notify_all();
		if (onCompleted)
			onCompleted();
	}
	void Task::Execute() {
		if (onExecuted)
			onExecuted();
	}
}