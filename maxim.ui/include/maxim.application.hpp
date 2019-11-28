#ifndef _maxim_application_hpp_
#define _maxim_application_hpp_

#include "maxim.event.hpp"
#include <maxim.io.hpp>
#include <atomic>

namespace maxim {

    template < class _sender_t >
    using key_down_event = event < _sender_t, input::key_info >;

    template < class _sender_t >
    using key_up_event = event < _sender_t, input::key_info >;

    class application {
	public:

		using application_start_event = event < application >;
		using application_close_event = event < application, bool & >;

		using application_draw_event = event < application, graphics & >;

		application_start_event on_start;
		application_close_event on_exit;

		application_draw_event on_draw;

		key_down_event < application >	on_key_down;
		key_up_event < application >	on_key_up;

		void run();

		void exit();

		maxim::device & device();

	private:

        class io_event_proxy : public maxim::input_event_handler {
        public:

            explicit io_event_proxy (maxim::application & app);

            void on_key_up (maxim::input::key_info const & key) override;
            void on_key_down (maxim::input::key_info const & key) override;

        private:
            maxim::application & _application;
        };

		maxim::device _device;

		std::atomic_bool _is_running { false };

        io_event_proxy _event_proxy {*this};
	};

}

#endif