#pragma once
#ifndef _maxim_details_hpp_
#define _maxim_details_hpp_

#include <cinttypes>
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace maxim {

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

		[[nodiscard]]
        inline bool is_zero () const { return x == 0 && y == 0; }
    };

    struct size {
        int32_t width, height;

		[[nodiscard]]
        inline bool is_zero () const { return width == 0 && height == 0; }

        inline bool operator == (size const & s) const { return width == s.width && height == s.height; }
        inline bool operator != (size const & s) const { return !operator==(s); }

		inline point as_point () const { return { width, height }; }
    };

    struct rect {
        maxim::point 	position;
        maxim::size		size;

	    inline rect operator + (point const & v) const { return { position + v, size }; }
	    inline rect operator - (point const & v) const { return { position - v, size }; }

	    [[nodiscard]]
        inline bool is_zero () const { return position.is_zero () && size.is_zero(); }

		[[nodiscard]]
        inline int32_t left () const { return position.x; }
		[[nodiscard]]
        inline int32_t right () const { return position.x + (size.width > 0 ? size.width - 1 : 0); }
		[[nodiscard]]
        inline int32_t top () const { return position.y; }
		[[nodiscard]]
        inline int32_t bottom () const { return position.y + (size.height > 0 ? size.height - 1 : 0); }
        [[nodiscard]]
        inline int32_t width () const { return size.width; }
        [[nodiscard]]
        inline int32_t height () const { return size.height; }

		[[nodiscard]]
        inline bool intersects (rect const & r) const {
			if (this->is_zero() || r.is_zero()) {
				return false;
			}

            return
                    left () <= r.right () &&
                    top () <= r.bottom () &&
                    right () >= r.left () &&
                    bottom () >= r.top ();
        }

		[[nodiscard]]
        inline rect intersection (rect const & r) const {
            if (!intersects(r))
                return {};

            point pos = {
                    std::max(left(), r.left()),
                    std::max(top(), r.top())
            };

            maxim::size size_v = {
                    std::min (right() + 1, r.right()) - pos.x + 1,
                    std::min (bottom() + 1, r.bottom()) - pos.y + 1
            };

            return { pos, size_v };
        }

        [[nodiscard]]
        inline bool contains (point const & p) const {
            return
                    p.x >= left() && p.x <= right() &&
                    p.y >= top() && p.y <= bottom();

        }
    };

    // enum helpers
    template < typename _enum_t, typename std::enable_if_t < std::is_enum_v < _enum_t >, int > = 0 >
    inline bool enum_has (_enum_t origin, _enum_t value) {
    	using under_t = std::underlying_type_t < _enum_t >;

    	auto cast_origin = static_cast < under_t > (origin);
    	auto cast_value = static_cast < under_t > (value);

    	return (cast_origin & cast_value) == cast_value;
    }

	template < typename ... enum_tv >
	inline auto enum_cat (enum_tv ... args) {
		static_assert ((std::is_enum_v < enum_tv > && ...), "enum_cat: all types must be enums");
		static_assert ((std::is_same_v < enum_tv, enum_tv > && ...), "enum_cat: all types must be the same enum");

		using enum_t = std::common_type_t < enum_tv... >;
		using underlying_t = std::underlying_type_t < enum_t >;

		return static_cast < enum_t > ((static_cast < underlying_t > (args) | ...));
	}

    template < typename type >
    struct span {
    public:

        using value_type =          type;
        using const_reference =     value_type const &;
        using const_pointer =       value_type const *;
        using size_type =           std::size_t;
        using const_iterator =      const_pointer;

        constexpr span () noexcept = default;
        constexpr span (span const & other) noexcept = default;
        constexpr span & operator = (span const &) = default;

        constexpr span (const_pointer ptr, size_type size) noexcept :
            _ptr (ptr),
            _size (size)
        {}

        /// Construct span from vector
        template < typename ... others_tv >
        constexpr explicit span (std::vector < value_type, others_tv... > const & vec) :
            _ptr (vec.data ()),
            _size (vec.size())
        {}

        template < size_type SIZE >
        constexpr explicit span (value_type const (&array)[SIZE]) :
            _ptr (array),
            _size (SIZE)
        {}

        template < typename it_type, typename end_type >
        constexpr span (it_type it, end_type end) noexcept :
            _ptr (&*it), // HACK: This is incredibly dangerous...
            _size (end - it)
        {}

        constexpr const_iterator begin () const noexcept {
            return this->_ptr;
        }

        constexpr const_iterator end () const noexcept {
            return this->_ptr + _size;
        }

        constexpr const_iterator cbegin () const noexcept {
            return this->_ptr;
        }

        constexpr const_iterator cend () const noexcept {
            return this->_ptr + _size;
        }

        constexpr const_reference operator [](std::size_t const POS) const noexcept {
            return *(this->_ptr + POS);
        }

        constexpr const_reference at (std::size_t const POS) const {
            if (POS >= this->_size) {
                throw std::out_of_range("view::at: pos value out of range");
            }

            return *(this->_ptr + POS);
        }

        constexpr const_reference front () const noexcept {
            return *(this->_ptr);
        }

        constexpr const_reference back () const noexcept {
            return *(this->_ptr + (this->_size - 1));
        }

        constexpr const_pointer data () const noexcept {
            return _ptr;
        }

        [[nodiscard]]
        constexpr size_type size () const noexcept {
            return _size;
        }

        [[nodiscard]]
        constexpr bool empty () const noexcept {
            return this->_size == 0;
        }

        constexpr void swap (span & other) noexcept {
            std::swap (_ptr, other._ptr);
            std::swap (_size, other._size);
        }

    private:
        const_pointer   _ptr = nullptr;
        size_type       _size = 0;
    };
}

#endif
