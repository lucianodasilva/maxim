#pragma once
#ifndef _maxim_key_handler_hpp_
#define _maxim_key_handler_hpp_

#include <maxim.io.hpp>
#include "maxim.event.hpp"

namespace maxim {

	class key_handler {
	public:
		virtual ~key_handler () = default;

		virtual void on_key_up (maxim::input::key_info const & key) = 0;
		virtual void on_key_down (maxim::input::key_info const & key) = 0;
		virtual void on_key_press (maxim::input::key_info const & key) = 0;
	};

	template < typename derived_type >
	using key_event = event < derived_type & /* sender */, input::key_info const & /* key */ >;

	template < typename derived_type /* work around virtual inheritance */ >
	class basic_key_handler : public key_handler {
	public:

		key_event < derived_type > key_up;
		key_event < derived_type > key_down;
		key_event < derived_type > key_press;

		void on_key_up (maxim::input::key_info const & key) override {
			key_up.invoke (*static_cast < derived_type * > (this), key);
		}

		void on_key_down (maxim::input::key_info const & key) override {
			key_down.invoke(*static_cast < derived_type * > (this), key);
		}

		void on_key_press (maxim::input::key_info const & key) override {
			key_press.invoke(*static_cast < derived_type * > (this), key);
		}
	};

}

#endif
