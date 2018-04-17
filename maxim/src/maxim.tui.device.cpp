#include "maxim.tui.device.hpp"

namespace maxim {
    namespace tui {

        point const & device::get_size () const {
            return _size;
        }

        character_buffer const & device::back_buffer () const {
            return _back_buffer;
        }

        device::device(std::unique_ptr<maxim::tui::driver_base> driver)
            : _driver(std::move(driver))
        {}

        void device::fullscreen(bool value) {
            _driver->set_alternate_buffer(value);
        }

        void device::enable_cursor(bool value) {
            _driver->set_cursor_enabled(value);
        }

        void device::clear() {
            //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear
            _back_buffer = {_size};
        }

        void device::draw_solid_box(
                color_t color,
                point const & position,
                point const & size)
        {
            character c = {};
            c.backcolor = color;
            c.content = ' ';

            for (uint32_t y = position.y; y < position.y + size.y; ++y) {
                uint32_t
                        line_begin = _back_buffer.coords_to_index({position.x, static_cast < int32_t > (y)}),
                        line_end = line_begin + size.x;

                for (uint32_t index = line_begin; index < line_end; ++index) {
                    _back_buffer.at(index) = c;
                }
            }
        }

        void device::draw_text(
                std::string const & text,
                point const & position,
                color_t color,
                color_t back_color,
                character_flags const & flags
        ){
            character c = {
                    ' ',
                    flags,
                    back_color,
                    color
            };

            auto index = _back_buffer.coords_to_index(position);

            for (uint32_t i = 0; i < text.size(); ++i) {
                c.content = text[i];
                _back_buffer.at (index + i) = c;
            }
        }

        void device::present() {
            auto size = _driver->get_size();

            if (_size != size) {
                _size = size;
                _back_buffer.resize(_size);
                _front_buffer.resize(_size);
            }

            _driver->encode_and_send (
                    _back_buffer,
                    _back_buffer.calculate_change_set(_front_buffer)
            );

            // swap buffers
            _front_buffer = _back_buffer;
        }

    }
}