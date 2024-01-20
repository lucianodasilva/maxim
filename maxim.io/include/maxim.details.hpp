#pragma once
#ifndef _maxim_io_details_hpp_
#define _maxim_io_details_hpp_

#include <cinttypes>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace maxim {

    template < typename type >
    struct vec2d {
    public:
        using value_type = type;

        [[nodiscard]]
        constexpr inline bool is_zero() const { return x == 0 && y == 0; }

        value_type x, y;
    };

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator+(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator-(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator*(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x * rhs.x, lhs.y * rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator*(vec2d<lhs_type> const& lhs, rhs_type const& rhs) {
        return { lhs.x * rhs, lhs.y * rhs };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator/(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return { lhs.x / rhs.x, lhs.y / rhs.y };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline vec2d<lhs_type> operator/(vec2d<lhs_type> const& lhs, rhs_type const& rhs) {
        return { lhs.x / rhs, lhs.y / rhs };
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline bool operator==(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    template <typename lhs_type, typename rhs_type>
    constexpr inline bool operator!=(vec2d<lhs_type> const& lhs, vec2d<rhs_type> const& rhs) {
        return !(lhs == rhs);
    }
    
    using point     = vec2d < int32_t >;
    using pointf    = vec2d < float >;

    struct size {
        public:
			constexpr size() = default;
			constexpr size(int32_t width, int32_t height) : width(width), height(height) {}
			constexpr size(point const& p) : width(p.x), height(p.y) {}

			[[nodiscard]]
			constexpr bool is_zero() const { return width == 0 && height == 0; }

			[[nodiscard]]
			constexpr point as_point() const { return { width, height }; }

            constexpr bool operator==(size const& rhs) const {
				return width == rhs.width && height == rhs.height;
			}

            constexpr bool operator!=(size const& rhs) const {
				return !operator==(rhs);
			}

			int32_t width{};
			int32_t height{};
    };

    struct rect {

        constexpr rect() = default;

        constexpr rect (int x, int y, int width, int height) : 
            x(x), y(y), width(width), height(height) {}

        constexpr rect (point const& position, maxim::size const& size) :
			x(position.x), y(position.y), width(size.width), height(size.height) {}

        [[nodiscard]]
        constexpr bool is_zero() const { return x == 0 && y == 0 && width == 0 && height == 0; }

        [[nodiscard]]
        constexpr point position() const { return { x, y }; }
		[[nodiscard]]
        constexpr maxim::size size() const { return { width, height }; }

		[[nodiscard]]

        [[nodiscard]]
        constexpr int32_t left() const { return x; }
        [[nodiscard]]
        constexpr int32_t right() const { return x + std::max < int32_t > (0, width - 1); }
        [[nodiscard]]
        constexpr int32_t top() const { return y; }
        [[nodiscard]]
        constexpr int32_t bottom() const { return y + std::max < int32_t > (0, height - 1); }

        [[nodiscard]]
        constexpr bool intersects(rect const& r) const {
            if (this->is_zero() || r.is_zero()) {
                return false;
            }

            return
                left() <= r.right() &&
                top() <= r.bottom() &&
                right() >= r.left() &&
                bottom() >= r.top();
        }

        [[nodiscard]]
        constexpr rect intersection(rect const& r) const {
            if (!intersects(r)) {
                return {};
            }

            point pos = {
                    std::max(left(), r.left()),
                    std::max(top(), r.top())
            };

            return { 
                pos.x,
                pos.y,
                std::min (right() + 1, r.right()) - pos.x + 1,
                std::min (bottom() + 1, r.bottom()) - pos.y + 1
            };
        }

        [[nodiscard]]
        constexpr bool contains(point const& p) const {
            return
                p.x >= left() && p.x <= right() &&
                p.y >= top() && p.y <= bottom();
        }

        [[nodiscard]]
        constexpr rect offset(point const& p) const {
			return { x + p.x, y + p.y, width, height };
		}

		[[nodiscard]]
        constexpr rect expand(point const& p) const {
            return { x, y, width + p.x, height + p.y };
        }
        
        int32_t x{};
        int32_t y{};
        int32_t width{};
        int32_t height{};
    };

    template < typename value_type >
    constexpr inline value_type lerp (value_type const& a, value_type const& b, float t) noexcept {
		return a + (b - a) * t;
	}

    template < typename value_type >
    constexpr inline vec2d < value_type > length (vec2d < value_type > const& value) noexcept {
        return std::sqrt (value.x * value.x + value.y * value.y);
	}

    template < typename value_type >
    constexpr inline float distance(vec2d < value_type > const& a, vec2d < value_type > const& b) noexcept {
		return length(a - b);
	}

    // enum helpers
    template < typename _enum_t, typename std::enable_if_t < std::is_enum_v < _enum_t >, int > = 0 >
    inline bool enum_has(_enum_t origin, _enum_t value) {
        using under_t = std::underlying_type_t < _enum_t >;

        auto cast_origin = static_cast <under_t> (origin);
        auto cast_value = static_cast <under_t> (value);

        return (cast_origin & cast_value) == cast_value;
    }

    template < typename ... enum_tv >
    inline auto enum_cat(enum_tv ... args) {
        static_assert ((std::is_enum_v < enum_tv > && ...), "enum_cat: all types must be enums");
        static_assert ((std::is_same_v < enum_tv, enum_tv > && ...), "enum_cat: all types must be the same enum");

        using enum_t = std::common_type_t < enum_tv... >;
        using underlying_t = std::underlying_type_t < enum_t >;

        return static_cast <enum_t> ((static_cast <underlying_t> (args) | ...));
    }
}

#endif
