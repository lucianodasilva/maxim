#include "maxim.encoder.hpp"
#include "maxim.character.hpp"
#include "maxim.stream.hpp"

namespace maxim {

    encoder & encoder::emmit_clear () {
        _stream.write (ansi::clear);
        return *this;
    }

    encoder & encoder::emmit_position(point const & p) {
        _stream.write_sequence(
            ansi::move_prefix,
            p.y + 1, ansi::separator, p.x + 1,
            ansi::move_suffix);

        return *this;
    }

    encoder & encoder::emmit_home() {
        return emmit_position ({0, 0});
    }

    encoder & encoder::emmit_attribute_reset() {
        _stream.write (ansi::attr_reset);
        return *this;
    }

    encoder & encoder::emmit_cursor(bool value) {
        _stream.write (value ? ansi::cursor_enable : ansi::cursor_disable);
        return *this;
    }

    encoder & encoder::emmit_graph(bool value) {
        _stream.write (value ? ansi::attr_enable_graph : ansi::attr_disable_graph);
        return *this;
    }

    encoder & encoder::emmit_wrap (bool value) {
        _stream.write (value ? ansi::wrap_enable : ansi::wrap_disable);
        return *this;
    }

    encoder & encoder::emmit_scrolling(bool value) {
        _stream.write (value ? ansi::scrolling_enable : ansi::scrolling_disable);
        return *this;
    }

    encoder & encoder::emmit_alternate_buffer(bool value) {
        _stream.write (value ? ansi::alternate_buffer_enable : ansi::alternate_buffer_disable);
        return *this;
    }

    encoder & encoder::emmit_color_4bit (struct color const & fg, struct color const & bg) {
        _stream.write_sequence (
            ansi::color_prefix,
            (int)fg.to_8(),
            ansi::separator,
            (int)(bg.to_8() + 10),
            ansi::color_suffix);

        return *this;
    }

    encoder & encoder::emmit_color_8bit (struct color const & fg, struct color const & bg) {
        _stream.write_sequence(
            ansi::color_prefix,
            ansi::color_foreground,
            (int)fg.to_256(),
            ansi::color_suffix);

        _stream.write_sequence(
            ansi::color_prefix,
            ansi::color_background,
            (int)bg.to_256(),
            ansi::color_suffix);

        return *this;
    }

    encoder & encoder::emmit_color_24bit(struct color const & fg, struct color const & bg) {
        _stream.write_sequence(
            ansi::color_prefix,
            ansi::color_foreground_24,
            (int)fg.r, ansi::separator, (int)fg.g, ansi::separator, (int)fg.b,
            ansi::color_suffix);

        _stream.write_sequence(
            ansi::color_prefix,
            ansi::color_background_24,
            (int)bg.r, ansi::separator, (int)bg.g, ansi::separator, (int)bg.b,
            ansi::color_suffix);

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
        _stream.write (ansi::color_reset);
        return *this;
    }

    encoder & encoder::encode_buffer (character_buffer const & buffer) {
        auto size = buffer.size ();

        this->emmit_attribute_reset();
        //this->emmit_color_reset ();

		if (!buffer.size().is_zero()) {
			// always emmit first color...
			this->emmit_color(buffer.ptr_at(0)->attributes.forecolor, buffer.ptr_at(0)->attributes.backcolor);
		}

	    character dummy {};
        character const * last = &dummy;

        for (int y = 0; y < size.height; ++y) {
            auto location = point { 0, y };

            this->emmit_position (location);
            auto * ptr = buffer.ptr_at (location);

            for (int x = 0; x < size.width; ++x) {

                // find and encode changes in attributes
                this->attribute_diff(last->attributes, ptr->attributes);

                // send content, replace zero with space
                _stream.write (ptr->content == 0 ? ' ' : ptr->content);

                last = ptr;
                ++ptr;
            }
        }

        return *this;
    }

    void encoder::send_out(output::driver* driver) {
        if (!driver) {
            return;
        }

		driver->send (_stream.data ());

		_stream.reset();
	}

    encoder & encoder::attribute_diff(const attributes &before, const attributes &now) {
        if (now == before)
            return *this;

		if (now.flags != before.flags) {
			// begin attribute data
			_stream.write(ansi::attr_prefix);

			if (now.flags.bold != before.flags.bold) {
				if (now.flags.bold) {
					_stream.write(ansi::attr_bold);
				} else {
					_stream.write(ansi::attr_bold_disable);
				}
			}

			if (now.flags.underlined != before.flags.underlined) {
				_stream
					.maybe_write(!now.flags.underlined, ansi::attr_disable_prefix)
					.write(ansi::attr_underline);
			}

			if (now.flags.blink != before.flags.blink) {
				_stream
					.maybe_write(!now.flags.blink, ansi::attr_disable_prefix)
					.write(ansi::attr_blink);
			}

			if (now.flags.reversed != before.flags.reversed) {
				_stream
					.maybe_write(!now.flags.reversed, ansi::attr_disable_prefix)
					.write(ansi::attr_reversed);
			}

			_stream.write(ansi::attr_suffix);
		}

	    if (now.is_graph != before.is_graph) {
		    if (now.is_graph) {
			    _stream.write(ansi::attr_enable_graph);
		    } else {
			    _stream.write(ansi::attr_disable_graph);
		    }
	    }

        if (before.backcolor != now.backcolor || before.forecolor != now.forecolor) {
            emmit_color (now.forecolor, now.backcolor);
        }

        return *this;
    }

    /*
    void encode_change_set (
        character_buffer const &		buffer,
        character_change_set const &	change_set,
        maxim::stream &			out_stream
    ) {
        if (change_set.empty())
            return;

        encode_attribute_reset(out_stream);
        character last = {};

        for (auto & change : change_set) {
            // move to starting point of change
            encode_move(out_stream, buffer.index_to_coords(change.begin));

            for (uint32_t index = change.begin; index < change.end; ++index) {
                auto & current = *buffer.ptr_at(index);
                // set attribute changes
                encode_attribute(out_stream, current.style, last.style);
                // set color changes
                encode_color(out_stream, current.style, last.style);
                // encode text
                out_stream.write(current.content);

                last = current;
            }
        }

        // after all changes committed reset attributes to avoid
        // terminal corruption on crash
        out_stream.write(ansi::color_reset);
        out_stream.write(ansi::attr_reset);
        out_stream.write(ansi::attr_disable_graph);
    }
    */

}