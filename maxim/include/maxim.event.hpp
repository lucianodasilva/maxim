#ifndef _MAXIM_EVENT_HPP_
#define _MAXIM_EVENT_HPP_

#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>

#include "maxim.event_dispatcher.hpp"

namespace maxim{
	namespace details {

		template < class _sender_t, class ... _args_tv >
		struct event_traits {
			using handler_t = std::function < void(_sender_t &, _args_tv && ...) >;
		};

		template < class _sender_t>
		struct event_traits < _sender_t, void >{
			using handler_t = std::function < void(_sender_t &) >;
		};

		template < class _sender_t, class ... _args_tv >
		struct event_data {
		public:
			using handler_t = typename details::event_traits < _sender_t, _args_tv...>::handler_t;
		protected:
			mutable std::mutex
				handler_mutex;

			std::vector < std::shared_ptr <handler_t> >
				handlers;
		};

		template < class _data_t, class _sender_t, class ... _args_tv >
		struct event_invoke : public _data_t
		{
		public:
			inline void invoke(_sender_t & sender, _args_tv && ... argv) const {
				std::remove_const_t < decltype (_data_t::handlers) > local_handlers;
				{
					std::lock_guard < std::mutex > lock(_data_t::handler_mutex);
					// copy handlers to avoid dead locks
					local_handlers = _data_t::handlers;
				}

				for (auto & h : local_handlers)
					h->operator ()(sender, std::forward<_args_tv...>(argv ...));
			}

			template < class dispatcher_t = event_dispatcher <> >
			inline void async_invoke(_sender_t & sender, _args_tv ... argv) {
				dispatcher_t::dispatch([=, handlers = _data_t::handlers]() {
					for (auto & h : handlers) {
						h(sender, argv...);
					}
				});
			}
		};

		template < class _data_t, class _sender_t >
		struct event_invoke < _data_t, _sender_t > : public _data_t
		{
		public:
			inline void invoke(_sender_t & sender ) const {
				std::remove_const_t < decltype (_data_t::handlers) > local_handlers;
				{
					std::lock_guard < std::mutex > lock(_data_t::handler_mutex);
					// copy handlers to avoid dead locks
					local_handlers = _data_t::handlers;
				}

				for (auto & h : local_handlers)
					h->operator ()(sender);
			}

			template < class dispatcher_t = event_dispatcher <> >
			inline void async_invoke(_sender_t & sender) {
				dispatcher_t::dispatch([sender, handlers = _data_t::handlers]() {
					for (auto & h : handlers) {
						h(sender);
					}
				});
			}
		};

	}

	template < class _sender_t, class ... _args_tv >
	struct event : public details::event_invoke <
		details::event_data < _sender_t, _args_tv...>,
		_sender_t,
		_args_tv...
	>{
	public:
		using handler_t = typename details::event_traits < _sender_t, _args_tv... >::handler_t;

		// force single instance / no copy
		event(const event &) = delete;
		event & operator = (const event &) = delete;

		inline event() {}

		inline void operator += (handler_t && handler) {
			std::lock_guard < std::mutex > lock(this->handler_mutex);
			this->handlers.emplace_back(
				std::make_shared < handler_t >(std::forward < handler_t > (handler))
			);
		}

		inline void operator -= (handler_t && handler) {
			std::lock_guard < std::mutex > lock(this->handler_mutex);

			auto it = find(
				this->handlers.begin(),
				this->handlers.end(),
				std::forward(handler)
			);

			if (it != this->handlers.end())
				this->handlers.erase(it);
		}

	};
}

#endif