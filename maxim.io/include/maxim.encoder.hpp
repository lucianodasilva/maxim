#pragma once
#ifndef MAXIM_ENCODER_HPP
#define MAXIM_ENCODER_HPP

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
            /// control sequence introducer
            CSI[]                       = "\033[",
            /// single shift select of G3
            SSE3[]                      = "\033O",
            /// reset to initial state
            RIS[]						= "\033c",
            /// goto to home position
            HOME[]						= "\033H",
            CLEAR[]                     = "\033[2J",
            ALTERNATE_BUFFER_ENABLE[]   = "\033[?1049h",
            ALTERNATE_BUFFER_DISABLE[]  = "\033[?1049l",
            SMCUP[]						= "\033[?47h",
            RMCUP[]						= "\033[?47l",
            CURSOR_ENABLE[]             = "\033[?25h",
            CURSOR_DISABLE[]            = "\033[?25l",
            SCROLLING_ENABLE[]			= "\033[?30h",
            SCROLLING_DISABLE[]			= "\033[?30l",
            WRAP_ENABLE[]				= "\033[?7h",
            WRAP_DISABLE[]				= "\033[?7l",
            MOVE_PREFIX[]               = "\033[",
            MOVE_SUFFIX[]               = "H",
            ATTR_PREFIX[]               = "\033[",
            ATTR_SUFFIX[]               = "m",
            ATTR_RESET[]                = "\033[0m",
            ATTR_BOLD[]                 = "1",
            ATTR_UNDERLINE[]            = "4",
            ATTR_BLINK[]                = "5",
            ATTR_REVERSED[]             = "7",
            ATTR_DISABLE_PREFIX[]       = "2",
            ATTR_ENABLE_GRAPH[]         = "\033(0",
            ATTR_DISABLE_GRAPH[]        = "\033(B",
            COLOR_RESET[]               = "\033[39;49m",
            COLOR_PREFIX[]              = "\033[",
            COLOR_FOREGROUND[]          = "38;5;",
            COLOR_FOREGROUND_24[]       = "38;2;",
            DEFAULT_FOREGROUND[]		= "39",
            COLOR_BACKGROUND[]          = "48;5;",
            COLOR_BACKGROUND_24[]       = "48;2;",
            DEFAULT_BACKGROUND[]		= "49",
            COLOR_SUFFIX[]              = "m",
            TERM[]                      = "~",
            SEPARATOR[]                 = ";";

        namespace graph_chars {
            constexpr char
                LINE_VERTICAL       {'x'},
                LINE_HORIZONTAL     {'q'},
                CROSS_LINES         {'n'},
                CORNER_UP_LEFT      {'j'},
                CORNER_UP_RIGHT     {'m'},
                CORNER_DOWN_LEFT    {'k'},
                CORNER_DOWN_RIGHT   {'l'},
                VERTICAL_LEFT       {'u'},
                VERTICAL_RIGHT      {'t'},
                HORIZONTAL_UP       {'v'},
                HORIZONTAL_DOWN     {'w'};
        }
    }

    struct encoder {
    public:
        encoder & emmit_clear ();
        encoder & emmit_position (maxim::point const & point);
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

        [[nodiscard]]
        inline maxim::color_mode color_mode () const { return _color_mode; }
        inline void color_mode (maxim::color_mode mode) { _color_mode = mode; }

        inline void reset () { _stream.reset (); }

        inline void send_out (output::driver * driver) {
            if (driver == nullptr) {
                return;
            }

            driver->send (_stream.data ());
            _stream.reset();
        }

    private:
        encoder & attribute_diff (character const & before, character const & now);

        maxim::stream       _stream;
        maxim::color_mode   _color_mode = color_mode::truecolor;
    };
}

#endif
