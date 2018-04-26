#include "maxim.tui.device.hpp"
#include "maxim.tui.encoding.hpp"

namespace maxim {
    namespace tui {

        point const & device::get_size () const {
            return _size;
        }

		maxim::tui::graphics device::graphics() {
			return { _back_buffer };
		}

        void device::fullscreen(bool value) {
			_stream.reset();

			if (value)
				_stream.write(encoding::ansi::alternate_buffer_enable);
			else
				_stream.write(encoding::ansi::alternate_buffer_disable);

			_driver.send(_stream);
        }

        void device::enable_cursor(bool value) {
			_stream.reset();

			if (value)
				_stream.write(encoding::ansi::cursor_enable);
			else
				_stream.write(encoding::ansi::cursor_disable);

			_driver.send(_stream);
        }

        void device::clear() {
            //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear
			graphics().clear();
        }

        void device::present() {
            auto size = _driver.get_size();

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

			_driver.send(_stream);

            // swap buffers
            _front_buffer = _back_buffer;

			_stream.reset();
        }

    }
}