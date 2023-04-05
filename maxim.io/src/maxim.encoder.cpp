#include "maxim.encoder.hpp"

namespace maxim {

    encoder & encoder::emmit_clear () {
        _stream.write (ansi::CLEAR);
        return *this;
    }

    encoder & encoder::emmit_position(point const & p) {
        _stream.write_sequence(
            ansi::MOVE_PREFIX,
            p.y + 1, ansi::SEPARATOR, p.x + 1,
            ansi::MOVE_PREFIX);

        return *this;
    }

    encoder & encoder::emmit_home() {
        return emmit_position ({0, 0});
    }

    encoder & encoder::emmit_attribute_reset() {
        _stream.write (ansi::ATTR_RESET);
        return *this;
    }

    encoder & encoder::emmit_cursor(bool value) {
        _stream.write (value ? ansi::CURSOR_ENABLE : ansi::CURSOR_DISABLE);
        return *this;
    }

    encoder & encoder::emmit_graph(bool value) {
        _stream.write (value ? ansi::ATTR_ENABLE_GRAPH : ansi::ATTR_DISABLE_GRAPH);
        return *this;
    }

    encoder & encoder::emmit_wrap (bool value) {
        _stream.write (value ? ansi::WRAP_ENABLE : ansi::WRAP_DISABLE);
        return *this;
    }

    encoder & encoder::emmit_scrolling(bool value) {
        _stream.write (value ? ansi::SCROLLING_ENABLE : ansi::SCROLLING_DISABLE);
        return *this;
    }

    encoder & encoder::emmit_alternate_buffer(bool value) {
        _stream.write (value ? ansi::ALTERNATE_BUFFER_ENABLE : ansi::ALTERNATE_BUFFER_DISABLE);
        return *this;
    }

    encoder & encoder::emmit_color_4bit (struct color const & fg, struct color const & bg) {
        _stream.write_sequence (
            ansi::COLOR_PREFIX,
            (int)fg.to_ansi_8(),
            ansi::SEPARATOR,
            (int)(bg.to_ansi_8() + 10),
            ansi::COLOR_SUFFIX);

        return *this;
    }

    encoder & encoder::emmit_color_8bit (struct color const & fg, struct color const & bg) {
        _stream.write_sequence(
            ansi::COLOR_PREFIX,
            ansi::COLOR_FOREGROUND,
            (int)fg.to_ansi_256(),
            ansi::COLOR_SUFFIX);

        _stream.write_sequence(
            ansi::COLOR_PREFIX,
            ansi::COLOR_BACKGROUND,
            (int)bg.to_ansi_256(),
            ansi::COLOR_SUFFIX);

        return *this;
    }

    encoder & encoder::emmit_color_24bit(struct color const & fg, struct color const & bg) {
        _stream.write_sequence(
            ansi::COLOR_PREFIX,
            ansi::COLOR_BACKGROUND_24,
            (int)fg.r, ansi::SEPARATOR, (int)fg.g, ansi::SEPARATOR, (int)fg.b,
            ansi::COLOR_SUFFIX);

        _stream.write_sequence(
            ansi::COLOR_PREFIX,
            ansi::COLOR_BACKGROUND_24,
            (int)bg.r, ansi::SEPARATOR, (int)bg.g, ansi::SEPARATOR, (int)bg.b,
            ansi::COLOR_SUFFIX);

        return *this;
    }

    encoder & encoder::emmit_color(struct color const & fg, struct color const & bg) {
        switch (_color_mode) {
            case color_mode::ansi_256:
                return emmit_color_8bit (fg, bg);
            case color_mode::truecolor:
                return emmit_color_24bit(fg, bg);
            default:
                return emmit_color_4bit (fg, bg);
        }
    }

    encoder & encoder::emmit_color_reset () {
        _stream.write (ansi::COLOR_RESET);
        return *this;
    }

    encoder & encoder::encode_buffer (character_buffer const & buffer) {
        auto size = buffer.size ();

        this->emmit_attribute_reset();
        this->emmit_color_reset ();

        character const * last {};

        for (int y = 0; y < size.height; ++y) {
            auto location = point { 0, y };

            this->emmit_position (location);
            auto const * ptr = buffer.data_at (location);

            for (int x = 0; x < size.width; ++x) {

                if (last != nullptr) {
                    // find and encode changes in attributes
                    this->attribute_diff(*last, *ptr);
                } else {
                    this->emmit_color(ptr->forecolor, ptr->backcolor);
                }

                // send content, replace zero with space
                _stream.write (ptr->content == 0 ? ' ' : ptr->content);

                last = ptr;
                ++ptr;
            }
        }

        return *this;
    }

    encoder & encoder::attribute_diff(character const & before,character const & now) {

        // write to stream with optional disabled prefix
        auto attr_write = [&stream = this->_stream](bool state, auto && attr) {
            if (!state) {
                stream.write(ansi::ATTR_DISABLE_PREFIX);
            }

            stream.write (attr);
        };

        // check if flags and colors are the same
        if (now.flags == before.flags &&
            has_same_colors(now, before))
        {
            return *this;
        }

        // begin attribute data
        _stream.write (ansi::ATTR_PREFIX);

        if (now.flags.bold != before.flags.bold) {
            attr_write (now.flags.bold, ansi::ATTR_BOLD);
        }

        if (now.flags.underlined != before.flags.underlined) {
            attr_write (now.flags.underlined, ansi::ATTR_UNDERLINE);
        }

        if (now.flags.blink != before.flags.blink) {
            attr_write (now.flags.blink, ansi::ATTR_BLINK);
        }

        if (now.flags.reversed != before.flags.reversed) {
            attr_write (now.flags.reversed, ansi::ATTR_REVERSED);
        }

        // end attribute data
        _stream.write(ansi::ATTR_SUFFIX);

        if (now.flags.is_graph != before.flags.is_graph) {
            emmit_graph (now.flags.is_graph);
        }

        if (before.backcolor != now.backcolor || before.forecolor != now.forecolor) {
            emmit_color (now.forecolor, now.backcolor);
        }

        return *this;
    }

}