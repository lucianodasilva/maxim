#include "maxim.tui.event_dispatcher.hpp"

namespace maxim {
	namespace tui {

		//void event_dispatcher::dispatch_task(std::function<void()> && f) {

		//	static auto dispatcher = event_dispatcher{
		//		std::thread {[&] {
		//			for (;;) {
		//				{
		//					std::lock_guard < std::mutex > lock(task_mutex);
		//					task_condition.wait(lock, [&]() -> bool { return tasks.empty(); });
		//					std::swap(tasks, executing);
		//				}

		//				// run executing tasks
		//				for (auto & t : executing) {
		//					t();
		//				}

		//				executing.resize(0);
		//			}
		//		}
		//	};

		//	static auto runner_thread = );

		//	{	// add task to task list
		//		std::lock_guard < std::mutex > lock(task_mutex);
		//		dispatcher._tasks.push_back(std::move(f));
		//	}

		//	dispatcher.task_condition.notify_one();
		//}

	}
}