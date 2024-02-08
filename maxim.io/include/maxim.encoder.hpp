#pragma once
#ifndef _maxim_io_encoder_hpp_
#define _maxim_io_encoder_hpp_

#include "maxim.character_buffer.hpp"
#include "maxim.output.driver.hpp"
#include "maxim.stream.hpp"

namespace maxim {

    /// Color mode
    enum struct color_mode {
        ansi_16,    /// 16 color mode (4 bit)
        ansi_256,   /// 256 color mode (8 bit)
        truecolor,  /// 24 bit color mode
    };

    namespace ansi {
        /// ANSI escape sequences
        /// \note https://www.xfree86.org/current/ctlseqs.html
        constexpr char const
            csi[] = "\033[",                            /// control sequence introducer
            ss3[] = "\033O",                            /// single shift select of G3
            ris[] = "\033c",                            /// reset to initial state
            home[] = "\033H",                           /// goto to home position
            clear[] = "\033[2J",
            alternate_buffer_enable[] = "\033[?1049h",
            alternate_buffer_disable[] = "\033[?1049l",
            smcup[] = "\033[?47h",
            rmcup[] = "\033[?47l",
            cursor_enable[] = "\033[?25h",
            cursor_disable[] = "\033[?25l",
            scrolling_enable[] = "\033[?30h",
            scrolling_disable[] = "\033[?30l",
            wrap_enable[] = "\033[?7h",
            wrap_disable[] = "\033[?7l",
            move_prefix[] = "\033[",
            move_suffix[] = "H",
            move_next_nline_suffix[] = "E",
            move_prev_nline_suffix[] = "F",
            insert_line_suffix[] = "L",
            delete_line_suffix[] = "M",
            attr_prefix[] = "\033[",
            attr_suffix[] = "m",
            attr_reset[] = "\033[0m",
            attr_bold[] = "1",
            attr_bold_disable[] = "22",
            attr_underline[] = "4",
            attr_blink[] = "5",
            attr_reversed[] = "7",
            attr_disable_prefix[] = "2",
            attr_enable_graph[] = "\033(0",             /// DEC VT100 Line Drawing Set
            attr_disable_graph[] = "\033(B",
            color_reset[] = "\033[39;49m",
            color_prefix[] = "\033[",
            color_foreground[] = "38;5;",
            color_foreground_24[] = "38;2;",
            default_foreground[] = "39",
            color_background[] = "48;5;",
            color_background_24[] = "48;2;",
            default_background[] = "49",
            color_suffix[] = "m",
            separator[] = ";";

        namespace graph {
            /// DEC VT100 Line Drawing Set
            constexpr char
                line_vertical = 'x',
                line_horizontal = 'q',
                cross_lines = 'n',
                corner_up_left = 'j',
                corner_up_right = 'm',
                corner_down_left = 'k',
                corner_down_right = 'l',
                vertical_left = 'u',
                vertical_right = 't',
                horizontal_up = 'v',
                horizontal_down = 'w';
        }
    }

    /// ANSI encoder
    struct encoder {
    public:

        /// Get stream instance
        maxim::stream & stream () { return _stream; }

        /// Emmit clear screen
        encoder & emmit_clear ();

        /// Emmit move cursor to position
        /// \param p position
        encoder & emmit_position (point const & p);

        /// Emmit move cursor to prev line
        /// \param line_count number of lines to move
        encoder & emmit_goto_prev (int line_count);

        /// Emmit move cursor to next line
        /// \param line_count number of lines to move
        encoder & emmit_goto_next (int line_count);

        /// Emmit move cursor to home position
        encoder & emmit_home ();

        /// Emmit reset attributes
        encoder & emmit_attribute_reset ();

        /// Emmit cursor enable/disable
        /// \param value true to enable, false to disable
        encoder & emmit_cursor (bool value);

        /// Emmit graph (DEC VT100 Line Drawing Set) enable/disable
        /// \param value true to enable, false to disable
        encoder & emmit_graph (bool value);

        /// Emmit wrap enable/disable
        /// \param value true to enable, false to disable
        encoder & emmit_wrap (bool value);

        /// Emmit scrolling enable/disable
        /// \param value true to enable, false to disable
        encoder & emmit_scrolling (bool value);

        /// Emmit alternate buffer enable/disable (kinda like fullscreen, preserving the "primary" buffer's contents)
        /// \param value true to enable, false to disable
        encoder & emmit_alternate_buffer (bool value);

        /// Emmit 4 bit color
        /// \param fg foreground color
        /// \param bg background color
        encoder & emmit_color_4bit (color const & fg, color const & bg);

        /// Emmit 8 bit color
        /// \param fg foreground color
        /// \param bg background color
        encoder & emmit_color_8bit (color const & fg, color const & bg);

        /// Emmit 24 bit color
        /// \param fg foreground color
        /// \param bg background color
        encoder & emmit_color_24bit (color const & fg, color const & bg);

        /// Emmit color
        /// \param fg foreground color
        /// \param bg background color
        /// \note The color mode is determined by the encoder's color mode
        encoder & emmit_color (color const & fg, color const & bg);

        /// Emmit color reset
        encoder & emmit_color_reset ();

        /// Add new line
        /// \param line_count - number of lines to insert
        encoder & emmit_new_line (int line_count);

        /// delete lines
        /// \param line_count - number of lines to delete
        encoder & emmit_delete_line (int line_count);

        /// Iterate through character buffer and emmit the visual data as ansi escape sequences
        /// \param buffer character buffer
        encoder & encode_buffer (character_buffer const & buffer);

        /// Send the encoded data to the output driver
        /// \param driver output driver
        inline void set_color_mode (color_mode mode) { _color_mode = mode; }

        /// Reset the encoder stream state
        inline void reset () { _stream.reset (); }

        /// Send the encoded data to the output driver
        void send_out(output::driver* driver);

    private:
        encoder & attribute_diff (attributes const & before, attributes const & now);

        maxim::stream   _stream;
        color_mode      _color_mode = color_mode::truecolor;
    };
}

#endif
