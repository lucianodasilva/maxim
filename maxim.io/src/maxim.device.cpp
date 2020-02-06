#include "maxim.device.hpp"
#include "maxim.encoding.hpp"

namespace maxim {

    void input_event_handler::on_key_up(const maxim::input::key_info &key) {}
    void input_event_handler::on_key_down(const maxim::input::key_info &key) {}

    device::device () {
		_stream.reset ();

		// setup defaults
		_stream.write (encoding::ansi::wrap_disable);
		_stream.write (encoding::ansi::scrolling_disable);

		_output_driver->send (_stream);
    }

    device::~device() {
    	_output_driver->flush();
        // restore default terminal configuration
		_stream.reset ();

		if (_alternate_buffer_enabled)
			encode_fullscreen(false, _stream);

		if (!_cursor_enabled)
			encode_cursor(true, _stream);

		_stream.write (encoding::ansi::wrap_enable);
		_stream.write (encoding::ansi::scrolling_enable);
		_stream.write (encoding::ansi::color_reset);
		_stream.write (encoding::ansi::attr_reset);
		_stream.write (encoding::ansi::attr_disable_graph);

		_output_driver->send (_stream);
    }

    size const & device::get_size () const {
        return _size;
    }

    character_buffer & device::backbuffer() {
        return _back_buffer;
    }

    maxim::stream & device::stream () {
    	return _stream;
    }

    void device::set_fullscreen(bool enabled) {
        maxim::stream stream;

        _alternate_buffer_enabled = enabled;
        encode_fullscreen(enabled, stream);

        _output_driver->send (stream);
    }

    void device::set_cursor_enabled (bool enabled) {
        maxim::stream stream;

        _cursor_enabled = enabled;
        encode_cursor (enabled, stream);

        _output_driver->send(stream);
    }

    void device::clear_screen() {
        //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear_screen
        graphics { _back_buffer }.clear();

        _stream.reset ();
        _stream.write(encoding::ansi::clear);
        _output_driver->send(_stream);
    }

    void device::present (bool full) {

		_stream.reset();


    	if (full) {
			encoding::encode (_back_buffer, _stream);
		} else {
			encoding::encode_change_set(
					_back_buffer,
					_back_buffer.calculate_change_set (_fore_buffer),
					_stream
			);

			std::swap (_back_buffer, _fore_buffer);
		}

        _output_driver->send(_stream);

        this->sync_display_settings();
    }

    void device::dispatch_input_events(maxim::input_event_handler & handler) const {
        _input_driver->dispatch_events(handler);
    }

	void device::encode_cursor(bool value, maxim::stream &stream) {
    	stream.write ((
    			value ?
    			encoding::ansi::cursor_enable :
    			encoding::ansi::cursor_disable));
    }

    void device::encode_fullscreen(bool value, maxim::stream &stream) {
		stream.write ((
				value ?
				encoding::ansi::alternate_buffer_enable :
				encoding::ansi::alternate_buffer_disable));
    }

    void device::sync_display_settings() {
    	_size = _output_driver->get_size ();

    	if (_back_buffer.size () != _size) {
			_back_buffer.resize(_size);
			_fore_buffer.resize(_size);

			_stream.reset ();
			_stream.write (encoding::ansi::clear);
			_stream.write (encoding::ansi::wrap_disable);
			_stream.write (encoding::ansi::scrolling_disable);

			if (!_cursor_enabled)
				device::encode_cursor (false, _stream);

			_output_driver->send (_stream);
		}
    }

}