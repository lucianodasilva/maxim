#pragma once
#ifndef _maxim_character_hpp_
#define _maxim_character_hpp_

#include <cinttypes>
#include <vector>
#include <tuple>

#include "maxim.character_change_set.hpp"
#include "maxim.details.hpp"

namespace maxim {

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

        inline character_buffer (maxim::size const & size) :
                _buffer { static_cast < size_t > (size.width * size.height) },
                _size { size }
        {}

        inline void reset () {
            _buffer.assign(_buffer.size(), character{});
        }

        inline character & at (point const & p) {
            return _buffer [coords_to_index(p)];
        }

        inline character const & at (point const & p) const {
            return _buffer [coords_to_index(p)];
        }

        inline character & at (size_t index) {
            return _buffer[index];
        }

        inline character const & at (size_t index) const {
            return _buffer[index];
        }

        inline maxim::size const & size() const { return _size; }

        inline void resize (maxim::size const & size) {
            std::vector < character > resized (
                    static_cast <size_t>(size.width * size.height));

            for (int32_t i = 0; i < size.height; ++i) {
                std::copy(
                        _buffer.begin() += (size.width * i),
                        _buffer.begin() += ((size.width * i) + size.width),
                        resized.begin() += (size.width * i)
                );
            }

            _buffer = resized;
            _size = size;
        }

        character_change_set calculate_change_set (
                character_buffer const & buffer
        ) const;

        inline int32_t coords_to_index (point const & p) const {
            return p.y * _size.width + p.x;
        }

        inline point index_to_coords (int32_t index) const {
            return {
                    static_cast < int32_t > (index % _size.width),
                    static_cast < int32_t > (index / _size.width)
            };
        }

    private:

        std::vector < character >
                _buffer;

        maxim::size
                _size;

    };
}

#endif
