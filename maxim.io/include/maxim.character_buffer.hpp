#pragma once
#ifndef _maxim_character_buffer_hpp_
#define _maxim_character_buffer_hpp_

#include "maxim.character.hpp"
#include "maxim.character_change_set.hpp"

namespace maxim {

	struct character_buffer {
	public:

		using character_type	= maxim::character;
		using reference			= character_type &;
		using pointer			= character_type *;
		using const_pointer		= character_type const *;

		inline explicit character_buffer(maxim::size size) :
				_buffer{static_cast < size_t > (size.width * size.height)},
				_size{size}
		{}

		inline void reset(maxim::color color = maxim::colors::black) {
			_buffer.assign(_buffer.size(), character{ color });
		}

		inline pointer data () { return _buffer.data (); }
		inline const_pointer data () const { return _buffer.data (); }

        inline reference at (point p) {
            auto idx = coords_to_index(p);

            if (idx > _buffer.size ()) {
				throw std::out_of_range("Point out of buffer bounds!");
            }

            return _buffer [idx];
        }

		inline pointer data_at(point p) {
			return _buffer.data() + coords_to_index(p);
		}

		inline const_pointer data_at(point p) const {
			return _buffer.data() + coords_to_index(p);
		}

		inline maxim::size size() const { return _size; }

		inline void resize(maxim::size size) {
			_buffer.resize(static_cast <size_t>(size.width * size.height));
			_size = size;

			reset();
		}

		[[nodiscard]]
		inline std::size_t coords_to_index(point p) const {
			return p.y * _size.width + p.x;
		}

		[[nodiscard]]
		inline point index_to_coords(int32_t index) const {
			return {
					static_cast < int32_t > (index % _size.width),
					static_cast < int32_t > (index / _size.width)
			};
		}

	private:

		std::vector <character_type> _buffer;
		maxim::size					 _size;
	};
}

#endif // _maxim_character_buffer_hpp_