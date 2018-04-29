#ifndef _MAXIM_TUI_STRUCTURES_HPP_
#define _MAXIM_TUI_STRUCTURES_HPP_

#include <cinttypes>
#include <algorithm>

namespace maxim {
	namespace tui {

		struct point {
			int32_t x, y;

			inline point operator + (point const & v) const { return { x + v.x, y + v.y }; }
			inline point operator - (point const & v) const { return { x - v.x, y - v.y }; }
			inline point operator * (point const & v) const { return { x * v.x, y * v.y }; }
			inline point operator * (int32_t s) const { return { x * s, y * s }; }
			inline point operator / (point const & v) const { return { x / v.x, y / v.y }; }
			inline point operator / (int32_t s) const { return { x / s, y / s }; }
			inline bool operator == (point const & p) const { return x == p.x && y == p.y; }
			inline bool operator != (point const & p) const { return !operator==(p); }

			inline bool is_zero () const { return x == 0 && y == 0; }
		};

		struct size {
			int32_t width, height;

			inline bool is_zero () const { return width == 0 && height == 0; }

			inline bool operator == (size const & s) const { return width == s.width && height == s.height; }
			inline bool operator != (size const & s) const { return !operator==(s); }

		};

		struct rect {
			maxim::tui::point 	position;
			maxim::tui::size	size;

			inline bool is_zero () const { return position.is_zero () && size.is_zero(); }

			inline int32_t left () const { return position.x; }
			inline int32_t right () const { return position.x + size.width; }
			inline int32_t top () const { return position.y; }
			inline int32_t bottom () const { return position.y + size.height; }

			inline bool intersects (rect const & r) const {
				return
						left () < r.right () &&
						top () < r.bottom () &&
						right () > r.left () &&
						bottom () > r.top ();
			}

			inline rect intersection (rect const & r) const {
				if (!intersects(r))
					return {};

				point pos = {
						std::max(left(), r.left()),
						std::max(top(), r.top())
				};

				maxim::tui::size size = {
						std::min (right(), r.right()) - pos.x,
						std::min (bottom(), r.bottom()) - pos.y
				};

				return { pos, size };
			}

			inline bool contains (point const & p) const {
				return
						p.x >= left() && p.x <= right() &&
						p.y >= top() && p.y <= bottom();

			}
		};


	}
}

#endif