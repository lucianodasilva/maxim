#include "maxim.tui.graphics.hpp"
#include "maxim.tui.encoding.hpp"

namespace maxim {
	namespace tui {

		graphics::graphics(character_buffer & buffer) : _buffer{ buffer }
		{}

		void graphics::clear() {
			_buffer = {_buffer.size()};
		}

		void graphics::draw_box(
			point const & position,
			point const & size,
			color_t color,
			bool solid,
			color_t backcolor
		){
			if (solid) {
				character c = {};
				c.backcolor = color;
				c.forecolor = color;
				c.content = ' ';

				for (int32_t y = position.y; y < position.y + size.y; ++y) {
					int32_t
						line_begin = _buffer.coords_to_index({ position.x, y }),
						line_end = line_begin + size.x;

					for (int32_t index = line_begin; index < line_end; ++index) {
						_buffer.at(static_cast < uint32_t > (index)) = c;
					}
				}
			}
			else {
				point top_left = position;
				point top_right = position + point{ size.x, 0 };
				point bottom_left = position + point{ 0, size.y };
				point bottom_right = position + size;

				// draw lines
				draw_h_line(top_left, size.x, color, false, backcolor);
				draw_h_line(bottom_left, size.x, color, false, backcolor);
				draw_v_line(top_left, size.y, color, false, backcolor);
				draw_v_line(top_right, size.y, color, false, backcolor);
				// draw corners
				character c = {
					' ', // content
				{
					false, // bold
					false, // undelined
					false, // blink
					false, // reversed
					true,  // is graph
				},
				backcolor,
				color
				};

				auto const & graphs = encoding::ansi::graph_chars;

				c.content = graphs.corner_up_right;
				_buffer.at(top_left) = c;
				c.content = graphs.corner_up_left;
				_buffer.at(top_right) = c;
				c.content = graphs.corner_down_right;
				_buffer.at(bottom_left) = c;
				c.content = graphs.corner_down_left;
				_buffer.at(bottom_right) = c;
			}
		}

		void graphics::draw_text(
			std::string const & text,
			point const & position,
			color_t color,
			color_t back_color,
			character_flags const & flags
		) {
			character c = {
				' ',
				flags,
				back_color,
				color
			};

			auto index = _buffer.coords_to_index(position);

			for (uint32_t i = 0; i < text.size(); ++i) {
				c.content = text[i];
				_buffer.at(index + i) = c;
			}
		}

		void graphics::draw_v_line(
			point const & position,
			int32_t length,
			color_t color,
			bool solid,
			color_t backcolor
		) {
			character c = {};

			if (solid) {
				c.content = ' ';
				c.backcolor = c.forecolor = color;
			}
			else {
				c.content = encoding::ansi::graph_chars.line_vertical;
				c.flags.is_graph = true;
				c.backcolor = backcolor;
				c.forecolor = color;
			}

			int32_t
				y1 = position.y,
				y2 = y1 + length;

			if (y2 < y1)
				std::swap(y1, y2);

			for (int32_t y = y1; y < y2; ++y) {
				_buffer.at({ position.x, y }) = c;
			}
		}

		void graphics::draw_h_line(
			point const & position,
			int32_t length,
			color_t color,
			bool solid,
			color_t backcolor
		) {
			character c = {};

			if (solid) {
				c.content = ' ';
				c.backcolor = c.forecolor = color;
			}
			else {
				c.content = encoding::ansi::graph_chars.line_horizontal;
				c.flags.is_graph = true;
				c.backcolor = backcolor;
				c.forecolor = color;
			}

			int32_t
				x1 = position.x,
				x2 = x1 + length;

			if (x2 < x1)
				std::swap(x1, x2);

			for (int32_t x = x1; x < x2; ++x)
				_buffer.at({ x, position.y }) = c;
		}


	}
}