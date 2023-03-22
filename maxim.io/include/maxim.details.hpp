#pragma once
#ifndef MAXIM_DETAILS_HPP
#define MAXIM_DETAILS_HPP

#include <cinttypes>
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace maxim {

    struct point {
        int32_t x, y;

        inline point operator + (point const & value) const { return { x + value.x, y + value.y }; }
        inline point operator - (point const & value) const { return { x - value.x, y - value.y }; }
        inline point operator * (point const & value) const { return { x * value.x, y * value.y }; }
        inline point operator * (int32_t scalar) const { return { x * scalar, y * scalar }; }
        inline point operator / (point const & value) const { return { x / value.x, y / value.y }; }
        inline point operator / (int32_t scalar) const { return { x / scalar, y / scalar }; }
        inline bool operator == (maxim::point const & other) const { return x == other.x && y == other.y; }
        inline bool operator != (maxim::point const & other) const { return !operator==(other); }

		[[nodiscard]]
        inline bool is_zero () const { return x == 0 && y == 0; }
    };

    struct size {
        int32_t width, height;

		[[nodiscard]]
        inline bool is_zero () const { return width == 0 && height == 0; }

        inline bool operator == (maxim::size const & size) const { return width == size.width && height == size.height; }
        inline bool operator != (maxim::size const & size) const { return !operator==(size); }

    };

    struct rect {
        maxim::point 	position;
        maxim::size		size;

		[[nodiscard]]
        inline bool is_zero () const { return position.is_zero () && size.is_zero(); }

		[[nodiscard]]
        inline int32_t left () const { return position.x; }
		[[nodiscard]]
        inline int32_t right () const { return position.x + size.width; }
		[[nodiscard]]
        inline int32_t top () const { return position.y; }
		[[nodiscard]]
        inline int32_t bottom () const { return position.y + size.height; }
        [[nodiscard]]
        inline int32_t width () const { return size.width; }
        [[nodiscard]]
        inline int32_t height () const { return size.height; }

		[[nodiscard]]
        inline bool intersects (maxim::rect const & other) const {
            return
                    left () < other.right () &&
                    top () < other.bottom () &&
                    right () > other.left () &&
                    bottom () > other.top ();
        }

		[[nodiscard]]
        inline rect intersection (rect const & other) const {
            if (!intersects(other)) {
                return {};
            }

            point const POS = {
                    std::max(left(), other.left()),
                    std::max(top(), other.top())
            };

            maxim::size const SIZE_V = {
                    std::min (right(), other.right()) - POS.x,
                    std::min (bottom(), other.bottom()) - POS.y
            };

            return { POS, SIZE_V };
        }

        [[nodiscard]]
        inline bool contains (point const & pnt) const {
            return
                    pnt.x >= left() && pnt.x < right() &&
                    pnt.y >= top() && pnt.y < bottom();

        }
    };

    // enum helpers
    template < typename enum_t, typename std::enable_if_t < std::is_enum_v < enum_t >, int > = 0 >
    inline bool enum_has (enum_t origin, enum_t value) { // NOLINT
    	using under_t = std::underlying_type_t < enum_t >;

    	auto cast_origin = static_cast < under_t > (origin);
    	auto cast_value = static_cast < under_t > (value);

    	return (cast_origin & cast_value) == cast_value;
    }
}

#endif
