#include "maxim.device.hpp"
#include "maxim.graphics.hpp"

namespace maxim {

    void input_event_handler::on_key_up(const maxim::input::key_info &key) {}
    void input_event_handler::on_key_down(const maxim::input::key_info &key) {}
	void input_event_handler::on_key_press(const maxim::input::key_info &key) {}

    device::device () {
        _encoder
            .emmit_wrap (false)
            .emmit_scrolling(false)
            .emmit_alternate_buffer(true)
            .emmit_cursor(false)
            .send_out (_output_driver.get ());
    }

    device::~device() {
        // restore default terminal configuration
		_encoder.reset ();

        _encoder
            .emmit_wrap (true)
            .emmit_alternate_buffer(false)
            .emmit_cursor (true)
            .emmit_scrolling(true)
            .emmit_color_reset ()
            .emmit_attribute_reset()
            .emmit_graph(false);

        _encoder.send_out (_output_driver.get());
    }

    size const & device::get_size () const {
        return _size;
    }

    character_buffer & device::backbuffer() {
        return _back_buffer;
    }

    maxim::encoder & device::encoder () {
    	return _encoder;
    }

    void device::set_color_mode (color_mode mode) {
        _encoder.set_color_mode (mode);
    }

    void device::clear_screen() {
        //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear_screen
        graphics { _back_buffer }.clear();

        _encoder.reset ();
        _encoder.emmit_clear ();
        _encoder.send_out (_output_driver.get ());
    }

    void device::present () {
		/*
        encoding::encode_change_set(
                _back_buffer,
                _back_buffer.calculate_change_set (_front_buffer),
                _stream
        );*/

        _encoder.reset ();
        _encoder.encode_buffer(_back_buffer);
        _encoder.send_out (_output_driver.get ());

        this->sync_display_settings();
    }

    void device::dispatch_input_events(maxim::input_event_handler & handler) const {
        _input_driver->dispatch_events(handler);
    }

    void device::sync_display_settings() {
    	_size = _output_driver->get_size ();

    	if (_back_buffer.size2d () != _size) {
			_back_buffer.resize(_size);

			_encoder.reset ();
            _encoder
                .emmit_wrap (false)
                .emmit_scrolling(false)
                .emmit_cursor (false);

            _encoder.send_out (_output_driver.get ());
		}
    }

}