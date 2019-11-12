#pragma once
#ifndef _maxim_encoding_hpp_
#define _maxim_encoding_hpp_

#include "maxim.stream.hpp"
#include "maxim.character.hpp"
#include "maxim.character_change_set.hpp"

namespace maxim::encoding {

    namespace ansi {
        constexpr char const
            clear[]                     = "\033[2J",
            alternate_buffer_enable[]   = "\033[?1049h",
            alternate_buffer_disable[]  = "\033[?1049l",
            cursor_enable[]             = "\033[?25h",
            cursor_disable[]            = "\033[?25l",
            move_prefix[]               = "\033[",
            move_suffix[]               = "H",
            attr_prefix[]               = "\033[",
            attr_suffix[]               = "m",
            attr_reset[]                = "\033[0m",
            attr_bold[]                 = "1",
            attr_underline[]            = "4",
            attr_blink[]                = "5",
            attr_reversed[]             = "7",
            attr_disable_prefix[]       = "2",
            attr_enable_graph[]         = "\033(0",
            attr_disable_graph[]        = "\033(B",
            color_prefix[]              = "\033[",
            color_foreground[]          = "38;5;",
            color_background[]          = "48;5;",
            color_suffix[]              = "m";

        constexpr graph_chars graph_chars = {
            'x', // line_vertical
            'q', // line_horizontal
            'n', // cross_lines
            'k', // corner_up_left
            'l', // corner_up_right
            'j', // corner_down_left
            'm', // corner_down_right
            'u', // vertical_left
            't', // vertical_right
            'v', // horizontal_up
            'w', // horizontal_down
        };
    }

    void encode_change_set (
        character_buffer const &		buffer,
        character_change_set const &	change_set,
        maxim::stream &			out_stream
    );
}

#endif
