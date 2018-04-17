#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include "maxim.tui.driver_xterm.hpp"

namespace maxim {
    namespace tui {

        void driver_xterm::xterm_flush_policy::flush(uint8_t const *buffer, size_t size) {
            ::write(STDOUT_FILENO, buffer, size);
        }

        template < class _t >
        auto as_integer (_t v) -> decltype(v + 1) {
            return +v;
        }

        constexpr char const
                xterm_clear []                      = "\033[2J",
                xterm_alternate_buffer_enable []    = "\033[?1049h",
                xterm_alternate_buffer_disable []   = "\033[?1049l",
                xterm_cursor_enable []              = "\033[?25h",
                xterm_cursor_disable []             = "\033[?25l",
                xterm_move_prefix []                = "\033[",
                xterm_move_suffix []                = "H",
                xterm_attr_prefix []                = "\033[",
                xterm_attr_suffix []                = "m",
                xterm_attr_reset []                 = "\033[0m",
                xterm_attr_bold []                  = "1",
                xterm_attr_underline []             = "4",
                xterm_attr_blink []                 = "5",
                xterm_attr_reversed []              = "7",
                xterm_attr_disable_prefix []        = "2",
                xterm_color_prefix []               = "\033[",
                xterm_color_foreground []           = "38;5;",
                xterm_color_background []           = "48;5;",
                xterm_color_suffix []               = "m";

        void driver_xterm::set_alternate_buffer(bool value) {
            if (value)
                _stream.immediate_write(xterm_alternate_buffer_enable);
            else
                _stream.immediate_write(xterm_alternate_buffer_disable);
        }

        void driver_xterm::set_cursor_enabled (bool value) {
            if (value)
                _stream.immediate_write(xterm_cursor_enable);
            else
                _stream.immediate_write(xterm_cursor_disable);
        }

        void driver_xterm::encode_and_send(
                character_buffer const & buffer,
                character_change_set const & changes
        ) {
            if (changes.size() == 0)
                return;

            encode_attribute_reset();
            character last = {};

            for (auto & change : changes) {
                // move to starting point of change
                encode_move(buffer.index_to_coords(change.begin));

                for (uint32_t index = change.begin; index < change.end; ++index) {
                    auto & current = buffer.at(index);
                    // set attribute changes
                    encode_attribute_diff(current.flags, last.flags);
                    // set color changes
                    encode_color_diff(current, last);
                    // encode text
                    _stream.write(current.content);

                    last = current;
                }
            }

            _stream.flush();
        }

        point driver_xterm::get_size() const {
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            return {
                w.ws_col,
                w.ws_row
            };
        }

        void driver_xterm::encode_move(maxim::tui::point const &p) {
            _stream
                    .write(xterm_move_prefix)
                    .write(p.y)
                    .write(";")
                    .write(p.x)
                    .write(xterm_move_suffix);
        }

        void driver_xterm::encode_attribute_reset () {
            _stream.write(xterm_attr_reset);
        }

        void driver_xterm::encode_attribute_diff (
                character_flags const & current,
                character_flags const & past
        ) {
            if (current.data == past.data)
                return;

            _stream.write(xterm_attr_prefix);

            if (current.bold != past.bold) {
                _stream
                        .optional_write(!current.bold, xterm_attr_disable_prefix)
                        .write(xterm_attr_bold);
            }

            if (current.underlined != past.underlined) {
                _stream
                        .optional_write(!current.underlined, xterm_attr_disable_prefix)
                        .write(xterm_attr_underline);
            }

            if (current.blink != past.blink) {
                _stream
                        .optional_write(!current.blink, xterm_attr_disable_prefix)
                        .write(xterm_attr_blink);
            }

            if (current.reversed != past.reversed) {
                _stream
                        .optional_write (!current.reversed, xterm_attr_disable_prefix)
                        .write(xterm_attr_reversed);
            }

            _stream.write(xterm_attr_suffix);
        }

        void driver_xterm::encode_color_diff(character const & current, character const & past) {

            bool
                forecolor_changed = current.forecolor != past.forecolor,
                backcolor_changed = current.backcolor != past.backcolor;

            if (!forecolor_changed && !backcolor_changed)
                return;

            _stream.write(xterm_color_prefix);

            if (forecolor_changed) {
                _stream
                        .write(xterm_color_foreground)
                        .write(as_integer(current.forecolor));

                if (backcolor_changed)
                    _stream.write(";");
            }

            if (backcolor_changed)
                _stream
                        .write(xterm_color_background)
                        .write(as_integer(current.backcolor));

            _stream.write(xterm_color_suffix);
        }

    }
}