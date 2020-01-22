#include "maxim.encoding.hpp"
#include "maxim.character.hpp"
#include "maxim.character_change_set.hpp"
#include "maxim.stream.hpp"

namespace maxim::encoding {

    template < class _t >
    auto encode_as_integer(_t v) -> decltype(v + 1) {
        return +v;
    }

    void encode_move(maxim::stream & stream, point const &p) {
        stream
            .write(ansi::move_prefix)
            .write(p.y + 1)
            .write(";")
            .write(p.x + 1)
            .write(ansi::move_suffix);
    }

    void encode_attribute_reset(maxim::stream & stream) {
        stream.write(ansi::attr_reset);
    }

    void encode_attribute(
        maxim::stream & stream,
        style_t const & current,
		style_t const & past
    ) {
        if (current._style_flags == past._style_flags)
            return;

        stream.write(ansi::attr_prefix);

        if (current.bold != past.bold) {
            stream
                .optional_write(!current.bold, ansi::attr_disable_prefix)
                .write(ansi::attr_bold);
        }

        if (current.underlined != past.underlined) {
            stream
                .optional_write(!current.underlined, ansi::attr_disable_prefix)
                .write(ansi::attr_underline);
        }

        if (current.blink != past.blink) {
            stream
                .optional_write(!current.blink, ansi::attr_disable_prefix)
                .write(ansi::attr_blink);
        }

        if (current.reversed != past.reversed) {
            stream
                .optional_write(!current.reversed, ansi::attr_disable_prefix)
                .write(ansi::attr_reversed);
        }

        stream.write(ansi::attr_suffix);

        if (current.is_graph != past.is_graph) {
            if (current.is_graph)
                stream.write(ansi::attr_enable_graph);
            else
                stream.write(ansi::attr_disable_graph);
        }

    }

    void encode_color(
        maxim::stream & stream,
        style_t const & current,
		style_t const & past
    ) {

        bool
            forecolor_changed = current.forecolor != past.forecolor,
            backcolor_changed = current.backcolor != past.backcolor;

        if (!forecolor_changed && !backcolor_changed)
            return;

        stream.write(ansi::color_prefix);

        if (forecolor_changed) {

        	if (current.forecolor == colors::system)
        		stream.write (ansi::default_foreground);
        	else
            	stream
                	.write(ansi::color_foreground)
                	.write(encode_as_integer(current.forecolor));

            if (backcolor_changed)
                stream.write(";");
        }

        if (backcolor_changed) {

        	if (current.backcolor == colors::system)
        		stream.write (ansi::default_background);
        	else
            	stream
                	.write(ansi::color_background)
                	.write(encode_as_integer(current.backcolor));
        }

        stream.write(ansi::color_suffix);
    }

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

        // after all changes commited reset attributes to avoid
        // terminal corruption on crash
        out_stream.write(ansi::color_reset);
        out_stream.write(ansi::attr_reset);
        out_stream.write(ansi::attr_disable_graph);
    }

}