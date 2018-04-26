#pragma once
#ifndef _MAXIM_TUI_CHARACTER_HPP_
#define _MAXIM_TUI_CHARACTER_HPP_

#include <cinttypes>
#include <vector>
#include <tuple>

#include "maxim.tui.character_change_set.hpp"
#include "maxim.tui.point.hpp"

namespace maxim {
    namespace tui {

        using color_t = uint8_t;

        namespace colors {
            constexpr color_t const
                    black       = 0x0,
                    dark_red    = 0x1,
                    dark_green  = 0x2,
                    dark_yellow = 0x3,
                    dark_blue   = 0x4,
                    purple      = 0x5,
                    dark_cyan   = 0x6,
                    gray        = 0x7,
                    dark_gray   = 0x8,
                    red         = 0x9,
                    green       = 0xA,
                    yellow      = 0xB,
                    blue        = 0xC,
                    pink        = 0xD,
                    cyan        = 0xE,
                    white       = 0xF;
        }

        union character_flags {
            struct {
                bool bold       : 1;
                bool underlined : 1;
                bool blink      : 1;
                bool reversed   : 1;
                bool is_graph   : 1;
                int             : 0; // fill to next byte
            };
            uint8_t data = {};
        };

        struct character {
            char content            = 0;
            character_flags flags   = {};
            color_t backcolor       = colors::black;
            color_t forecolor       = colors::white;
        };

        struct graph_chars {
            char
                    line_vertical,
                    line_horizontal,
                    cross_lines,
                    corner_up_left,
                    corner_up_right,
                    corner_down_left,
                    corner_down_right,
                    vertical_left,
                    vertical_right,
                    horizontal_up,
                    horizontal_down;
        };

        inline bool operator == (character const & c1, character const & c2) {
            return
                c1.content == c2.content &&
                c1.flags.data == c2.flags.data &&
                c1.backcolor == c2.backcolor &&
                c1.forecolor == c2.forecolor;
        }

        struct character_buffer {
        public:

            inline character_buffer (point const & size) :
                    _buffer { static_cast < long unsigned int > (size.x * size.y) },
                    _size { size }
            {}

            inline character & at (point const & p) {
                return _buffer [coords_to_index(p)];
            }

            inline character const & at (point const & p) const {
                return _buffer [coords_to_index(p)];
            }

            inline character & at (uint32_t index) {
                return _buffer[index];
            }

            inline character const & at (uint32_t index) const {
                return _buffer[index];
            }

			inline point const & size() const { return _size; }

            inline void resize (point const & size) {
                std::vector < character > resized (size.x * size.y);

                for (int32_t i = 0; i < size.y; ++i) {
                    std::copy(
                            _buffer.begin() += (size.x * i),
                            _buffer.begin() += ((size.x * i) + size.x),
                            resized.begin() += (size.x * i)
                    );
                }

                _buffer = resized;
                _size = size;
            }

            character_change_set calculate_change_set (
                    character_buffer const & buffer
            ) const;

            inline uint32_t coords_to_index (point const & p) const {
                return p.y * _size.x + p.x;
            }

            inline point index_to_coords (uint32_t index) const {
                return {
                        static_cast < int32_t > (index % _size.x),
                        static_cast < int32_t > (index / _size.x)
                };
            }

        private:

            std::vector < character >
                    _buffer;

            point   _size;

        };

    }
}

#endif //_MAXIM_TUI_CHARACTER_HPP_
