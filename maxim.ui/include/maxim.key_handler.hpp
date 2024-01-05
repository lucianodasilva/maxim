#pragma once
#ifndef _maxim_key_handler_hpp_
#define _maxim_key_handler_hpp_

#include <maxim.io.hpp>
#include "maxim.event.hpp"

namespace maxim {

	class key_handler_base {
	public:
		virtual ~key_handler_base () = default;

		virtual void on_key_up (maxim::input::key_info const & key) = 0;
		virtual void on_key_down (maxim::input::key_info const & key) = 0;
		virtual void on_key_press (maxim::input::key_info const & key) = 0;
	};

	template < typename sender_type >
	using key_event = event < sender_type & /* sender */, input::key_info const & /* key */ >;

	template < typename sender_type, typename base_type = key_handler_base /* work around virtual inheritance */ >
	class key_handler : public base_type {
	public:

		key_event < sender_type > key_up;
		key_event < sender_type > key_down;
		key_event < sender_type > key_press;

		void on_key_up (maxim::input::key_info const & key) override {
			key_up.invoke (*static_cast < sender_type * > (this), key);
		}

		void on_key_down (maxim::input::key_info const & key) override {
			key_down.invoke(*static_cast < sender_type * > (this), key);
		}

		void on_key_press (maxim::input::key_info const & key) override {
			key_press.invoke(*static_cast < sender_type * > (this), key);
		}
	};

}

#endif
