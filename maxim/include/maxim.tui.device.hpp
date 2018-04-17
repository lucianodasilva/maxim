#pragma once
#ifndef _MAXIM_TUI_DEVICE_HPP_
#define _MAXIM_TUI_DEVICE_HPP_

#include "maxim.tui.character.hpp"
#include "maxim.tui.driver_xterm.hpp"

namespace maxim {
    namespace tui {

        using default_driver = driver_xterm;

        template < class _driver_t = default_driver >
        struct device {
        public:

            inline void set_fullscreen(bool value) {
                _driver.set_alternate_buffer(value);
            }

            inline void enable_cursor (bool value) {
                _driver.set_cursor_enabled (value);
            }

            inline void clear() {
                //TODO: check if sending the clear signal causes flickering and replace this with xterm code for clear
                _back_buffer = {_size};
            }

            inline void draw_solid_box (
                    color_t color,
                    point const & position,
                    point const & size
            ) {
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

            inline void draw_text (
                    std::string const & text,
                    point const & position,
                    color_t color = colors::white,
                    color_t back_color = colors::black,
                    character_flags const & flags = {}
            ) {
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

            inline void present () {
                auto size = _driver.get_size();

                if (_size != size) {
                    _size = size;
                    _back_buffer.resize(_size);
                    _front_buffer.resize(_size);
                }

                _driver.encode_and_send (
                    _back_buffer,
                    _back_buffer.calculate_change_set(_front_buffer)
                );

                // swap buffers
                _front_buffer = _back_buffer;
            }

            inline point get_size () const { return _size; }

            inline character_buffer & back_buffer () {
                return _back_buffer;
            }

        private:

            _driver_t   _driver;
            point       _size           { _driver.get_size() };

            character_buffer
                        _back_buffer    { _size },
                        _front_buffer   { _size };

        };

    }
}

#endif //_MAXIM_TUI_DEVICE_HPP_
