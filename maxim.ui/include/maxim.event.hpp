#pragma once
#ifndef _maxim_ui_event_h_
#define _maxim_ui_event_h_

#include <algorithm>
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "maxim.traits.hpp"

namespace maxim {

	/// Represents the life cycle of an event observer.
	struct event_guard  {
	public:

		// erase copying and assignment
		event_guard(event_guard const &) = delete;
		event_guard & operator=(event_guard const &) = delete;

		class observer_guard_base;

		template<typename observer_t>
		class observer_guard;

		/** @brief Disable the scope guard
		 *
		 * Release the reference to the event observer without destroing it
		 */
		void dismiss();

		/** @brief Release observer
		 *
		 * Release the reference to the event observer and destroy it
		 */
		void reset();

		event_guard() = default;

		event_guard(event_guard &&other) noexcept;

		template<typename observer_t>
		inline explicit event_guard(std::shared_ptr<observer_t> observer) :
			_observer{std::make_unique<observer_guard<observer_t> >(observer)} {};

		event_guard &operator=(event_guard &&other) noexcept;

		class observer_guard_base {
		public:
			virtual void dismiss() = 0;

			virtual ~observer_guard_base() = default;
		};

		template<typename observer_t>
		class observer_guard : public observer_guard_base {
		public:

			void dismiss() override {
				weak_observer.reset();
			}

			explicit observer_guard(std::shared_ptr<observer_t> observer) :
				weak_observer{observer} {}

			~observer_guard() override {
				auto observer = weak_observer.lock();

				if (observer) {
					observer->release();
					weak_observer.reset();
				}
			}

			std::weak_ptr<observer_t> weak_observer;
		};

		inline void swap(event_guard &other) noexcept {
			std::swap(_observer, other._observer);
		}

	private:
		std::unique_ptr<observer_guard_base> _observer;
	};

	template<typename ... args_t>
	struct event {
	public:

		event () = default;

		// disable copying and assignment
		event(event const &) = delete;
		event &operator=(event const &) = delete;

		using observer_id_t = std::size_t;

		class observer_base {
		public:

			inline explicit observer_base(event &owner_ref, observer_id_t id_v) :
				owner{owner_ref},
				ID{id_v} {};

			virtual void invoke(args_t ...) = 0;

			void release() {
				owner.remove(ID); //WARNING: no guarantee this object exists after this line, do not add code after this
				released = true;
			}

			event &owner;
			observer_id_t const ID;
			std::atomic_bool released{false};
		};

		template<typename callback_t>
		class observer : public observer_base {
		public:
			inline explicit observer(callback_t callback, event &owner_ref, observer_id_t id) :
				_callback(std::move(callback)),
				observer_base(owner_ref, id) {}

			void invoke(args_t ... args_v) override {
				if (!this->released) {
					_callback(std::forward<args_t>(args_v)...);
				}
			}

		private:
			callback_t _callback;
		};

		inline void invoke(args_t ... args) {
			decltype(_observers) local_observers;

			{
				std::unique_lock<std::mutex> lock(_internal_mutex);

				if (_observers.empty()) {
					return;
				}

				local_observers = _observers;
			}

			for (auto &observer: local_observers) {
				observer->invoke(args...);
			}
		}

		template<typename callback_t>
		[[nodiscard]]
		inline event_guard append(callback_t && callback) {
			std::unique_lock<std::mutex> lock(_internal_mutex);
			return append_impl(std::forward<callback_t>(callback));
		}

		template<typename callback_t>
		inline observer_id_t append_no_scope(callback_t && callback) {
			std::unique_lock<std::mutex> lock(_internal_mutex);

			auto guard{append_impl(std::forward<callback_t>(callback))};
			guard.dismiss();

			// return current id,
			return _observer_tick;
		}

		inline bool remove(observer_id_t id) {
			std::unique_lock<std::mutex> lock(_internal_mutex);

			auto erase_it = std::remove_if(
				_observers.begin(), _observers.end(),
				[id](std::shared_ptr<observer_base> &item) {
					return item->ID == id;
				});

			if (erase_it == _observers.end()) {
				return false;
			}

			_observers.erase(erase_it);

			return true;
		}

	private:

		template<typename callback_t>
		inline event_guard append_impl(callback_t &&callback) {
			auto &obs_instance = _observers.emplace_back(
				std::make_shared<observer<callback_t> >(
					callback,
					*this,
					++_observer_tick));

			return event_guard(obs_instance);
		}

		std::vector<std::shared_ptr<observer_base> >
			_observers{};
		observer_id_t _observer_tick{0};

		std::mutex _internal_mutex;
	};

}

#endif
