#include "maxim.graphics.hpp"
#include "maxim.encoding.hpp"

namespace maxim {

	graphics::graphics(character_buffer &buffer) :
		_buffer{buffer},
		view{{}, buffer.size()} {}

	graphics::graphics(character_buffer &buffer, rect const &view)
		: _buffer{buffer}, view{view} {}

	void graphics::clear() {
		_buffer.reset();
	}

	void graphics::draw_box(
		rect const &rct,
		color_t color,
		bool solid
	) {
		if (solid) {
			character c = {};
			c.style.backcolor = color;

			for (
				int32_t y = rct.top();
				y < rct.bottom();
				++y
				) {
				for (int32_t x = rct.left(); x < rct.right(); ++x) {
					set({x, y}, c);
				}
			}
		} else {

			point
				top_left = rct.position,
				top_right = {rct.right(), rct.top()},
				bottom_left = {rct.left(), rct.bottom()},
				bottom_right = {rct.right(), rct.bottom()};

			// draw lines
			draw_h_line(top_left, rct.size.width, color, false);
			draw_h_line(bottom_left, rct.size.width, color, false);
			draw_v_line(top_left, rct.size.height, color, false);
			draw_v_line(top_right, rct.size.height, color, false);

			auto const &graphs = encoding::ansi::graph_chars;

			auto set_corner = [color] (character * c, char content) {
				if (!c)
					return;

				c->content = content;
				c->style.is_graph = true;
				c->style.forecolor = color;
			};

			set_corner (get(top_left), graphs.corner_down_right);
			set_corner (get(top_right), graphs.corner_down_left);
			set_corner (get(bottom_left), graphs.corner_up_right);
			set_corner (get(bottom_right), graphs.corner_up_left);
		}
	}

	void graphics::draw_text(
		std::string const &text,
		point const &position,
		style_t style
	) {

		character c = {
			style,
			' '
		};

		int32_t len = static_cast <int32_t> (text.size());

		for (int32_t i = 0; i < len; ++i) {
			c.content = text[i];
			set(position + point{i, 0}, c);
		}
	}

	void graphics::draw_text(
		std::string const &text,
		point const &position,
		int32_t width,
		alignment align,
		style_t style
	) {
		character c = {
			style,
			' '
		};

		int32_t
			len = std::min(static_cast <int32_t> (text.size()), width),
			left_pad{0};

		if (align == alignment::right) {
			left_pad = width - len;
		} else if (align == alignment::centered) {
			auto half_width = width / 2;
			auto half_len = len / 2;

			left_pad = half_width - half_len;
		}

		// pad left
		c.content = ' ';
		for (int32_t i = 0; i < left_pad; ++i)
			set(position + point{i, 0}, c);

		// text
		for (int32_t i = 0; i < len; ++i) {
			c.content = text[i];
			set(position + point{left_pad + i, 0}, c);
		}

		c.content = ' ';

		// pad right
		for (int32_t i = left_pad + len; i < width; ++i) {
			set(position + point{i, 0}, c);
		}
	}

	void graphics::draw_v_line(
		point const &position,
		int32_t length,
		color_t color,
		bool solid
	) {
		character c = {};

		if (solid) {
			c.style.backcolor = color;
		} else {
			c.content = encoding::ansi::graph_chars.line_vertical;
			c.style.is_graph = true;
			c.style.forecolor = color;
		}

		for (int32_t i = 0; i < length; ++i)
			set(position + point{0, i}, c);
	}

	void graphics::draw_h_line(
		point const &position,
		int32_t length,
		color_t color,
		bool solid
	) {
		character c = {};

		if (solid) {
			c.style.backcolor = color;
		} else {
			c.content = encoding::ansi::graph_chars.line_horizontal;
			c.style.is_graph = true;
			c.style.forecolor = color;
		}

		for (int32_t i = 0; i < length; ++i)
			set(position + point{i, 0}, c);
	}

	void graphics::draw_buffer(
		character_buffer const &buffer,
		point const &position,
		rect source,
		bool null_mask
	) {
		if (source.is_zero())
			source = {{}, buffer.size()};

		for (int32_t y = source.top(); y < source.bottom(); ++y) {
			for (int32_t x = source.left(); x < source.right(); ++x) {
				auto offset = point{x, y};
				auto &c = buffer.at(offset);

				if (c.content != 0)
					set(position + offset, c);
			}
		}
	}
}