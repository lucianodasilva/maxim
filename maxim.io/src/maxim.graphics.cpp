#include "maxim.graphics.hpp"
#include "maxim.encoder.hpp"

#include <memory>

namespace maxim {

	graphics::graphics(character_buffer &buffer) :
		_buffer{buffer}
	{
		clip_window_push({ {}, _buffer.size() });
	}

	graphics::graphics(character_buffer &buffer, rect const & clip_window)
		: _buffer{buffer}
	{
		clip_window_push(clip_window);
	}

	void graphics::clear(struct color clear_color) {
		_buffer.reset(clear_color);
	}

    void graphics::draw_point (maxim::point const & position, struct color color) {
        // transform
        auto dest = position + transform();
		auto clip = this->clip_rect ();

        if (clip.contains (dest)) {
            character c;
            c.attributes.backcolor = color;

            *_buffer.ptr_at (dest) = c;
        }
    }

    void graphics::draw_char (maxim::point const & position, struct color color, char c, attribute_flags flags, bool is_graph) {
        // transform
	    auto dest = position + transform();
	    auto clip = this->clip_rect ();

	    if (clip.contains (dest)) {
            auto * cref = _buffer.ptr_at (dest);

            cref->attributes.forecolor = color;
            cref->attributes.flags = flags;
			cref->attributes.is_graph = is_graph;
            cref->content = c;
        }
    }

	character * graphics::get_char (point const & position) {
		// transform
		auto dest = position + transform();
		auto clip = this->clip_rect();

		if (clip.contains(dest)) {
			return _buffer.ptr_at(dest);
		} else {
			return nullptr;
		}
	}

	void graphics::draw_box(
		rect const & rct,
		struct color color,
		bool solid
	) {
		auto t_rect = transform_rect (rct);
		auto clip = this->clip_rect ();

		auto clipped = clip.intersection (t_rect);

		if (solid) {
			if (clipped.is_zero ()) {
				return;
			}

			character c {};
			c.attributes.backcolor = color;

			for (int32_t y = clipped.top (); y <= clipped.bottom(); ++y) {
				auto * ptr = _buffer.ptr_at ({clipped.left (), y});
				std::fill(ptr, ptr + clipped.size.width, c);
			}
		} else {

			point
				top_left = rct.position,
				top_right = {rct.right(), rct.top()},
				bottom_left = {rct.left(), rct.bottom()},
				bottom_right = {rct.right(), rct.bottom()};

			// draw lines
            if (rct.size.width > 2) {
                draw_h_line(top_left, rct.size.width, color, false);
                draw_h_line(bottom_left, rct.size.width, color, false);
            }

            if (rct.size.height > 2) {
                draw_v_line(top_left, rct.size.height, color, false);
                draw_v_line(top_right, rct.size.height, color, false);
            }

			auto const & graphs = ansi::graph_chars;

			auto set_corner = [&] (point pos, char content) {
				if (!clipped.contains (pos + transform ()))  {
					return;
				}

				auto *c = _buffer.ptr_at(pos);

				if (!c)
					return;

				c->content = content;
				c->attributes.is_graph = true;
				c->attributes.forecolor = color;
			};

			set_corner (top_left, graphs.corner_down_right);
			set_corner (top_right, graphs.corner_down_left);
			set_corner (bottom_left, graphs.corner_up_right);
			set_corner (bottom_right, graphs.corner_up_left);
		}
	}

	void graphics::draw_text(
		std::string const &text,
		point const &position,
		struct color color
	) {
		this->draw_text (
				text,
				{
					position,
					{ static_cast < int > (text.size()), 1 }
				},
                color,
				text_attributes {}
				);
	}

	void graphics::draw_text(
			std::string const &text,
			point const &position,
			int32_t width,
            struct color color,
			text_attributes attributes
	) {
		this->draw_text (
				text,
				{ position, width, 1 },
                color,
				attributes);
	}

	inline auto get_next_line(
			std::string const& text,
			int begin,
			int line_width)
	{
		if (begin + line_width < text.size ()) {

			int end = begin + line_width;

			while (end > 0 && std::isgraph(text[end - 1]))
				--end;

			if (end - begin <= 0)
				return std::make_tuple(begin, begin + line_width);

			return std::make_tuple(begin, end);
		} else {
			return std::make_tuple (begin, static_cast < int > (text.size ()));
		}
	}

	inline int get_line_count (std::string const & text, int line_width) {
		int offset {0};
		int line_count {0};

		int begin {0};
		int end {0};

		while (end < text.size ()) {
			std::tie (begin, end) = get_next_line(text, offset, line_width);
			offset = end;
			++line_count;
		}

		return line_count;
	}

	inline point get_text_position (rect const & bounds, alignment line_alignment, int line_count) {
		switch (line_alignment) {
			case (alignment::near):
				return bounds.position;
			case (alignment::far):
				return bounds.position + point{ 0, static_cast <int> (bounds.size.height - line_count) };
			default:
				return bounds.position + point{ 0, static_cast <int> (bounds.size.height / 2 - (line_count / 2)) };
		}
	}

	inline point get_line_position (rect const & bounds, alignment alignment, int line, int line_width) {
		int x = 0;

		switch (alignment) {
			case (alignment::center):
				x = bounds.size.width / 2 - line_width / 2;
				break;
			case(alignment::far):
				x = bounds.size.width - line_width;
				break;
			default:
				x = 0;
				break;
		}

		return bounds.position + point { x, line };
	}

	void graphics::draw_text(
			std::string const &text,
			rect const & bounds,
            struct color color,
			text_attributes attributes
	) {
		// transform and clip
		auto t_bounds = transform_rect (bounds);
		auto text_position = t_bounds.position;

		auto clip = this->clip_rect ();

		auto clipped = clip.intersection (t_bounds);

		if (clipped.is_zero())
			return;

		if (attributes.wrap) {
			// calculate wrap rect
			auto lines = get_line_count (text, t_bounds.size.width);

			text_position = get_text_position(
					t_bounds,
					attributes.v_align,
					lines);

			int offset = 0;
			for (int line = 0; line < lines; ++line) {
				auto [ 
					line_begin,
					line_end
				] = get_next_line(text, offset, t_bounds.size.width);

				auto line_position = get_line_position (
						{ text_position, t_bounds.size },
						attributes.h_align,
						line,
						line_end - line_begin);

				// write to buffer
				for (int i = line_begin; i < line_end; ++i) {
					if (clipped.contains (line_position)) {
                        auto * ch = _buffer.ptr_at (line_position);
                        ch->attributes.forecolor = color;
                        ch->attributes.flags = attributes.flags;
                        ch->content = text [i];
                    }

					++line_position.x;
				}

				offset = line_end;
			}
		} else {
			text_position = get_text_position(
					t_bounds,
					attributes.v_align,
					1);

			int left_pad = 0;

			if (attributes.h_align == alignment::far) {
				left_pad = t_bounds.size.width - text.size();
			} else if (attributes.h_align == alignment::center) {
				auto half_width = t_bounds.size.width / 2;
				auto half_len = text.size() / 2;

				left_pad = half_width - half_len;
			}

			// text
			for (int i = 0; i < text.size(); ++i) {

				auto p = text_position + point { left_pad + i, 0};

				if (clipped.contains (p)) {
                    auto *ch = _buffer.ptr_at(p);
                    ch->attributes.forecolor = color;
                    ch->attributes.flags = attributes.flags;
                    ch->content = text[i];
                }
			}
		}

	}

	void graphics::draw_v_line(
		point const &position,
		int32_t length,
		struct color color,
		bool solid
	) {
		auto t_rect = transform_rect ({ position, { 1, length }});
		auto clip = this->clip_rect ();

		auto clipped = clip.intersection (t_rect);

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.attributes.backcolor = color;
		} else {
			c.content = ansi::graph_chars.line_vertical;
			c.attributes.is_graph = true;
			c.attributes.forecolor = color;
		}

		for (int32_t y = clipped.top (); y <= clipped.bottom(); ++y) {
			*_buffer.ptr_at ({clipped.left (), y}) = c;
		}
	}

	void graphics::draw_h_line(
		point const &position,
		int32_t length,
		struct color color,
		bool solid
	) {
		auto t_rect = transform_rect ({ position, {  length, 1 }});
		auto clip = this->clip_rect ();

		auto clipped = clip.intersection (t_rect);

		if (clipped.is_zero())
			return;

		character c = {};

		if (solid) {
			c.attributes.backcolor = color;
		} else {
			c.content = ansi::graph_chars.line_horizontal;
			c.attributes.is_graph = true;
			c.attributes.forecolor = color;
		}

		auto * ptr = _buffer.ptr_at (clipped.position);
		std::fill (ptr, ptr + clipped.size.width, c);
	}

	void graphics::transform_push (point const & transform) {
		_transform_stack.push (this->transform () + transform);
	}

	void graphics::transform_pop() {
		if (!_transform_stack.empty ()) {
			_transform_stack.pop ();
		}
	}

	point graphics::transform () const{
		if (!_transform_stack.empty ()) {
			return _transform_stack.top ();
		} else {
			return {};
		}
	}

	void graphics::clip_window_push(const maxim::rect &clip) {
		_clip_stack.push (clip);
	}

	void graphics::clip_window_pop () {
		if (!_clip_stack.empty ()) {
			_clip_stack.pop ();
		}
	}

	rect graphics::clip_window () const {
		if (!_clip_stack.empty ()) {
			return _clip_stack.top ();
		} else {
			return { {}, _buffer.size () };
		}
	}

}