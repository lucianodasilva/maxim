#include "maxim.device.hpp"
#include "maxim.encoding.hpp"

namespace maxim {

    void input_event_handler::on_key_up(const maxim::input::key_info &key) {}
    void input_event_handler::on_key_down(const maxim::input::key_info &key) {}

    device::~device() {

        // restore default terminal configuration
        if (_alternate_buffer_enabled)
            set_fullscreen(false);

        if (!_cursor_enabled)
            set_cursor_enabled(true);
    }

    size const & device::get_size () const {
        return _size;
    }

    character_buffer & device::backbuffer() {
        return _back_buffer;
    }

    void device::set_fullscreen(bool enabled) {
        _stream.reset();

        if (enabled)
            _stream.write(encoding::ansi::alternate_buffer_enable);
        else
            _stream.write(encoding::ansi::alternate_buffer_disable);

        _alternate_buffer_enabled = enabled;

        _output_driver->send(_stream);
    }

    void device::set_cursor_enabled (bool enabled) {
        _stream.reset();

        if (enabled)
            _stream.write(encoding::ansi::cursor_enable);
        else
            _stream.write(encoding::ansi::cursor_disable);

        _cursor_enabled = enabled;

        _output_driver->send(_stream);
    }

    void device::clear_screen() {
        //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear_screen
        graphics { _back_buffer }.clear();

        _stream.reset ();
        _stream.write(encoding::ansi::clear);

        _output_driver->send(_stream);
    }

    void device::present() {
        auto size = _output_driver->get_size();

        if (_size != size) {
            _size = size;
            _back_buffer.resize(_size);
            _front_buffer.resize(_size);
        }

        _stream.reset();

        encoding::encode_change_set(
            _back_buffer,
            _back_buffer.calculate_change_set (_front_buffer),
            _stream
        );

        _output_driver->send(_stream);

        // swap buffers
        _front_buffer = _back_buffer;

        _stream.reset();
    }

    void device::dispatch_input_events(maxim::input_event_handler & handler) const {
        _input_driver->dispatch_events(handler);
    }

}