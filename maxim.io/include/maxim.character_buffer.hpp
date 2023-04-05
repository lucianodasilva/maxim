#pragma once
#include "las/view.hpp"
#ifndef MAXIM_CHARACTER_BUFFER_HPP
#define MAXIM_CHARACTER_BUFFER_HPP

#include <cstddef>

#include "maxim.character.hpp"

namespace maxim {

	struct character_buffer {
	public:

		using character_type	= maxim::character;
		using reference			= character_type &;
		using pointer			= character_type *;
		using const_pointer		= character_type const *;
        using view_type         = maxim::view < character_type >;
        using const_view_type   = maxim::view < character_type const >;

		inline explicit character_buffer(maxim::size size) :
				_buffer{static_cast < size_t > (size.width * size.height)},
				_size{size}
		{}

		inline void reset(maxim::color color = maxim::colors::BLACK) {
			_buffer.assign(_buffer.size(), character{ color });
		}

        [[nodiscard]]
		inline view_type data () {
            return { _buffer.data(), _buffer.size() };
        }

		[[nodiscard]]
        inline const_view_type data () const {
            return {_buffer.data(), _buffer.size()};
        }

        [[nodiscard]]
        inline reference at (maxim::point point) {
            auto idx = coords_to_index(point);

            if (idx > _buffer.size ()) {
				throw std::out_of_range("Point out of buffer bounds!");
            }

            return _buffer [idx];
        }

        [[nodiscard]]
		inline pointer data_at(maxim::point point) {
			return _buffer.data() + coords_to_index(point);
		}

		[[nodiscard]]
        inline const_pointer data_at(maxim::point point) const {
			return _buffer.data() + coords_to_index(point);
		}

        [[nodiscard]]
		inline maxim::size size() const { return _size; }

		inline void resize(maxim::size size) {
			_buffer.resize(static_cast < buffer_type::size_type > (size.width) * size.height);
			_size = size;

			reset();
		}

		[[nodiscard]]
		inline std::size_t coords_to_index(maxim::point point) const {
			return point.y * _size.width + point.x;
		}

		[[nodiscard]]
		inline point index_to_coords(int32_t index) const {
			return {
					static_cast < int32_t > (index % _size.width),
					static_cast < int32_t > (index / _size.width)
			};
		}

	private:

        using buffer_type = std::vector < character_type >;

		buffer_type _buffer;
		maxim::size	_size;
	};
}

#endif