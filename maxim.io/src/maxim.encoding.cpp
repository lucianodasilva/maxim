#include "maxim.encoding.hpp"
#include "maxim.character.hpp"
#include "maxim.character_change_set.hpp"
#include "maxim.stream.hpp"

namespace maxim::encoding {

    template < class _t >
    inline auto encode_as_integer(_t v) -> decltype(v + 1) {
        return +v;
    }

    inline void encode_move(maxim::stream & stream, point const &p) {
        stream
            .write(ansi::move_prefix)
            .write(p.y + 1)
            .write(";")
            .write(p.x + 1)
            .write(ansi::move_suffix);
    }

	inline void encode_home(maxim::stream & stream) {
		encode_move(stream, {0,0});
	}

    inline void encode_attribute_reset(maxim::stream & stream) {
        stream.write(ansi::attr_reset);
    }

    inline void encode_attribute(
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

    inline void encode_color(
        maxim::stream & stream,
        style_t const & current,
		style_t const & past
    ) {
    	if (current.forecolor != past.forecolor) {
			stream.write(ansi::color_prefix);

			if (current.forecolor == colors::system)
				stream.write(ansi::default_foreground);
			else
				stream
						.write(ansi::color_foreground)
						.write(encode_as_integer(current.forecolor));

			stream.write(ansi::color_suffix);
		}

    	if (current.backcolor != past.backcolor) {
			stream.write(ansi::color_prefix);

			if (current.backcolor == colors::system)
				stream.write(ansi::default_background);
			else
				stream
						.write(ansi::color_background)
						.write(encode_as_integer(current.backcolor));


			stream.write(ansi::color_suffix);
		}
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

    void encode (character_buffer const & buffer, maxim::stream & out_stream) {
		auto size = buffer.size ();

		out_stream.write (ansi::color_reset);


		character last;

		for (int y = 0; y < size.height; ++y) {
			auto location = point { 0, y };

			encode_move (out_stream, location);
			auto * ptr = buffer.ptr_at (location);

			for (int x = 0; x < size.width; ++x) {
				// find and encode changes in style
				encode_attribute(out_stream, ptr->style, last.style);
				encode_color (out_stream, ptr->style, last.style);

				// send content
				out_stream.write (ptr->content);

				last = *ptr;
				++ptr;
			}
		}

		// after all changes committed reset attributes to avoid
		// terminal corruption on crash
		//encode_attribute_reset(out_stream);

    }

}