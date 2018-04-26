#ifndef _MAXIM_TUI_GRAPHICS_HPP_
#define _MAXIM_TUI_GRAPHICS_HPP_

#include "maxim.tui.character.hpp"
#include "maxim.tui.point.hpp"

namespace maxim {
	namespace tui {

		struct graphics {

			graphics(character_buffer & buffer);

			void clear();

			void draw_box(
				point const & position,
				point const & size,
				color_t color,
				bool solid = true,
				color_t backcolor = colors::black
			);

			void draw_text(
				std::string const & text,
				point const & position,
				color_t color = colors::white,
				color_t back_color = colors::black,
				character_flags const & flags = {}
			);

			void draw_v_line(
				point const & position,
				int32_t length,
				color_t color,
				bool solid = true,
				color_t backcolor = colors::black
			);

			void draw_h_line(
				point const & position,
				int32_t length,
				color_t color,
				bool solid = true,
				color_t backcolor = colors::black
			);

		private:
			character_buffer & _buffer;
		};

	}
}

#endif // _MAXIM_TUI_GRAPHICS_HPP_