#pragma once
#ifndef _maxim_character_buffer_hpp_
#define _maxim_character_buffer_hpp_

#include "maxim.character.hpp"

namespace maxim {

	struct character_buffer {
	public:

		inline explicit character_buffer(maxim::size const &size) :
				_buffer{static_cast < size_t > (size.width * size.height)},
				_size{size} {}

		inline void reset(struct color color) {
			_buffer.assign(_buffer.size(), character { color });
		}

		inline character *ptr_at(point const &p) {
			return ptr_at(coords_to_index(p));
		}

		inline character const *ptr_at(point const &p) const {
			return ptr_at(coords_to_index(p));
		}

		inline character *ptr_at(size_t index) {
			return _buffer.data() + index;
		}

		inline character const *ptr_at(size_t index) const {
			return _buffer.data() + index;
		}

		inline maxim::size const &size() const { return _size; }

		inline void resize(maxim::size const &size) {
			std::vector <character> resized(static_cast <size_t>(size.width * size.height));

			_buffer = resized;
			_size = size;
		}

		// [[nodiscard]]
		// character_change_set calculate_change_set(
		//	 	character_buffer const &buffer
		// ) const;

		[[nodiscard]]
		inline std::size_t coords_to_index(point const &p) const {
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

		std::vector <character>
				_buffer;

		maxim::size
				_size;
	};
}

#endif // _maxim_character_buffer_hpp_