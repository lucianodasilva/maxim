#include "maxim.device.hpp"
#include "maxim.encoding.hpp"

namespace maxim {

    device::~device() {

        // restore default terminal configuration
        if (_alternate_buffer_enabled)
            fullscreen(false);

        if (!_cursor_enabled)
            enable_cursor(true);
    }

    size const & device::get_size () const {
        return _size;
    }

    character_buffer & device::backbuffer() {
        return _back_buffer;
    }

    maxim::graphics device::graphics() {
        return { _back_buffer };
    }

    void device::fullscreen(bool value) {
        _stream.reset();

        if (value)
            _stream.write(encoding::ansi::alternate_buffer_enable);
        else
            _stream.write(encoding::ansi::alternate_buffer_disable);

        _alternate_buffer_enabled = value;

        _output_driver->send(_stream);
    }

    void device::enable_cursor(bool value) {
        _stream.reset();

        if (value)
            _stream.write(encoding::ansi::cursor_enable);
        else
            _stream.write(encoding::ansi::cursor_disable);

        _cursor_enabled = value;

        _output_driver->send(_stream);
    }

    void device::clear() {
        //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear
        graphics().clear();

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

    std::vector<input::key_event_args> device::get_input_events() const {
        return _input_driver->get_input_events();
    }
}