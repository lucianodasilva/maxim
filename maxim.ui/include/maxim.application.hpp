#pragma once
#ifndef _maxim_ui_application_hpp_
#define _maxim_ui_application_hpp_

#include <maxim.io.hpp>

#include "maxim.event.hpp"
#include "maxim.focus.hpp"
#include "maxim.key_handler.hpp"
#include "maxim.paintable.hpp"
#include "maxim.style.hpp"

namespace maxim {

	class control;

	using application_start_event = event < class application & >;
	using application_close_event = event < class application &, bool & >;

	enum struct application_paint_order {
		pre_controls,
		post_controls
	};

	class application :
		public  basic_paintable < application >,
		public  basic_key_handler < application >,
	    public  focus_manager,
		private maxim::input_event_handler
	{
	public:
		application_start_event started;
		application_close_event exiting;

		property < maxim::style > style = style::default_style();
		property < application_paint_order > paint_order = application_paint_order::pre_controls;
		property < bool > close_on_empty_active_queue = true;

		application () = default;

		void run();
		void stop();

		maxim::device & device();

		template < typename control_type, typename ... args_tv, std::enable_if_t < std::is_base_of_v < maxim::control, control_type >, int > = 0 >
		control_type * push_active (args_tv && ... args) {
			auto uptr = std::make_unique < control_type > (
				std::forward < args_tv > (args)...);

			uptr->attach_to_tree (this, nullptr);

			// set root focus index
			uptr->focus_index (focus_index().append (0));

			do_deactivate();
			auto * ptr = uptr.get();
			_active_stack.push_back(std::move(uptr));
			do_activate();

			return ptr;
		}

		void pop_active ();

		class control * active () const {
			if (_active_stack.empty()) {
				return nullptr;
			}

			return _active_stack.back().get();
		}

	private:
		void on_key_up (maxim::input::key_info const & key) override;
		void on_key_down (maxim::input::key_info const & key) override;

		void on_key_press (maxim::input::key_info const & key) override;

		void on_paint (maxim::graphics & g, maxim::size size) override;

		static void on_style_changed (application & sender, maxim::style & new_style);

		void do_activate ();
		void do_deactivate ();

		std::vector < std::unique_ptr < maxim::control > >
							_active_stack;

		maxim::device       _device;
		std::atomic_bool    _is_running { false };
	};
}

#endif
