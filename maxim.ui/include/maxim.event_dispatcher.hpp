#pragma once
#ifndef _maxim_event_dispatcher_hpp_
#define _maxim_event_dispatcher_hpp_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>
#include <thread>

namespace maxim {
	namespace details {

		using event_dispatcher_task = std::function < void () >;

		template < class _mutex_t >
		struct event_dispatcher_traits {
			using mutex_t = _mutex_t;
			using condition_variable_t = std::condition_variable_any;
		};

		template <>
		struct event_dispatcher_traits < std::mutex > {
			using mutex_t = std::mutex;
			using condition_variable_t = std::condition_variable;
		};

	}

	template < class _mutex_t = std::mutex >
	struct event_dispatcher : details::event_dispatcher_traits < _mutex_t > {
	public:

		static void dispatch(details::event_dispatcher_task && t) {
			static event_dispatcher dispatcher{};

			{
				std::unique_lock < typename base_t::mutex_t > lock(dispatcher._task_mutex);
				dispatcher._tasks.push_back(std::move(t));
			}

			dispatcher._task_condition.notify_one();
		}

	private:

		using base_t = details::event_dispatcher_traits < _mutex_t >;

		typename base_t::mutex_t
			_task_mutex;

		typename base_t::condition_variable_t
			_task_condition;

		std::thread
			_task_thread;

		std::vector < details::event_dispatcher_task >
			_tasks,
			_executing_tasks;

		std::atomic < bool >
			_run{ true };

		inline event_dispatcher() {
			_task_thread = std::thread([this] {
				for (;;) {
					{
						std::unique_lock < typename base_t::mutex_t > lock(_task_mutex);

						if (!this->_run && this->_tasks.empty())
							return;

						_task_condition.wait(
							lock,
							[this] { return !(this->_tasks.empty() && this->_run); }
						);

						std::swap(this->_tasks, this->_executing_tasks);
						this->_tasks.resize(0);
					}

					for (auto & task : this->_tasks)
						task();
				}
			});
		}

		~event_dispatcher() {
			_run = false;

			if (_task_thread.joinable())
				_task_thread.join();

			_task_condition.notify_all();
		}

	};

}

#endif