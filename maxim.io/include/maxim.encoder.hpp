#pragma once
#ifndef _maxim_encoder_hpp_
#define _maxim_encoder_hpp_

#include "maxim.character_buffer.hpp"
#include "maxim.output.driver.hpp"
#include "maxim.stream.hpp"

#include <type_traits>

namespace maxim {

    enum struct color_mode {
        ansi_16,
        ansi_256,
        truecolor,
    };

    namespace ansi {

        constexpr char const
            csi[]                       = "\033[", // control sequence introducer
            sse3[]                      = "\033O", // single shift select of G3
            ris[]						= "\033c", // reset to initial state
            home[]						= "\033H", // goto to home position
            clear[]                     = "\033[2J",
            alternate_buffer_enable[]   = "\033[?1049h",
            alternate_buffer_disable[]  = "\033[?1049l",
            smcup[]						= "\033[?47h",
            rmcup[]						= "\033[?47l",
            cursor_enable[]             = "\033[?25h",
            cursor_disable[]            = "\033[?25l",
            scrolling_enable[]			= "\033[?30h",
            scrolling_disable[]			= "\033[?30l",
            wrap_enable[]				= "\033[?7h",
            wrap_disable[]				= "\033[?7l",
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
            color_reset[]               = "\033[39;49m",
            color_prefix[]              = "\033[",
            color_foreground[]          = "38;5;",
            color_foreground_24[]       = "38;2;",
            default_foreground[]		= "39",
            color_background[]          = "48;5;",
            color_background_24[]       = "48;2;",
            default_background[]		= "49",
            color_suffix[]              = "m",
            separator[]                 = ";";

        namespace graph_chars {
            constexpr char
                line_vertical       {'x'},
                line_horizontal     {'q'},
                cross_lines         {'n'},
                corner_up_left      {'j'},
                corner_up_right     {'m'},
                corner_down_left    {'k'},
                corner_down_right   {'l'},
                vertical_left       {'u'},
                vertical_right      {'t'},
                horizontal_up       {'v'},
                horizontal_down     {'w'};
        }
    }

    struct encoder {
    public:
        encoder & emmit_clear ();
        encoder & emmit_position (point const & p);
        encoder & emmit_home ();

        encoder & emmit_attribute_reset ();

        encoder & emmit_cursor (bool value);
        encoder & emmit_graph (bool value);
        encoder & emmit_wrap (bool value);
        encoder & emmit_scrolling (bool value);
        encoder & emmit_alternate_buffer (bool value);

        encoder & emmit_color_4bit (color const & fg, color const & bg);
        encoder & emmit_color_8bit (color const & fg, color const & bg);
        encoder & emmit_color_24bit (color const & fg, color const & bg);

        encoder & emmit_color (color const & fg, color const & bg);
        encoder & emmit_color_reset ();

        encoder & encode_buffer (character_buffer const & buffer);

        inline void set_color_mode (color_mode mode) { _color_mode = mode; }

        inline void reset () { _stream.reset (); }

        inline void send_out (output::driver * driver) {
            if (!driver) {
                return;
            }

            driver->send (_stream.data ());
            _stream.reset();
        }

    private:

        encoder & attribute_diff (character const & before, character const & now);

        maxim::stream   _stream;
        color_mode      _color_mode = color_mode::truecolor;
    };
}

#endif
