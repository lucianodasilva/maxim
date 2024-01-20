#pragma once
#ifndef _maxim_io_character_buffer_hpp_
#define _maxim_io_character_buffer_hpp_

#include "maxim.character.hpp"
#include <vector>

namespace maxim {

	/// Character Buffer strut ( 2D array of characters )
	struct character_buffer {
	public:

		/// Constructor
		/// \param size - 2D Size of the buffer
		explicit character_buffer(maxim::size const &size) :
				_buffer{static_cast < size_t > (size.width * size.height)},
				_size{size} {}

		/// Clear buffer contents with a given color
		/// \param color - Color to clear with
		void clear (struct color color) {
			_buffer.assign(_buffer.size(), character { color });
		}

		/// Get pointer to character at a given 2d position
		/// \param p - 2D Position
		/// \return Pointer to character
		/// \note This function does not check for bounds. If the buffer is resized, the pointer may become invalid.
		character *ptr_at(point const &p) {
			return ptr_at(coords_to_index(p));
		}

		/// Get const pointer to character at a given 2d position
		/// \param p - 2D Position
		/// \return Pointer to character
		/// \note This function does not check for bounds. If the buffer is resized, the pointer may become invalid.
		character const *ptr_at(point const &p) const {
			return ptr_at(coords_to_index(p));
		}

		/// Get pointer to character at a given unidimentional index
		/// \param index - Index
		/// \return Pointer to character
		/// \note This function does not check for bounds. If the buffer is resized, the pointer may become invalid.
		character *ptr_at(size_t index) {
			return _buffer.data() + index;
		}

		/// Get const pointer to character at a given unidimentional index
		/// \param index - Index
		/// \return Pointer to character
		/// \note This function does not check for bounds. If the buffer is resized, the pointer may become invalid.
		character const *ptr_at(size_t index) const {
			return _buffer.data() + index;
		}

		/// Buffer 2d size
		maxim::size const &size() const { return _size; }

		/// Resize buffer
		/// \param size - New size
		/// \note This function does not preserve buffer contents nor addresses. All pointers to characters become invalid.
		void resize(maxim::size const &size) {
			std::vector <character> resized(static_cast <size_t>(size.width * size.height));

			_buffer = resized;
			_size = size;
		}

		// [[nodiscard]]
		// character_change_set calculate_change_set(
		//	 	character_buffer const &buffer
		// ) const;

		/// Convert 2d coordinates to unidimentional index
		/// \param p - 2d coordinates
		/// \return Unidimentional index
		[[nodiscard]] std::size_t coords_to_index(point const &p) const {
			return p.y * _size.width + p.x;
		}

		/// Convert unidimentional index to 2d coordinates
		/// \param index - Unidimentional index
		/// \return 2d coordinates
		[[nodiscard]] point index_to_coords(int32_t index) const {
			return {
					static_cast < int32_t > (index % _size.width),
					static_cast < int32_t > (index / _size.width)
			};
		}

	private:
		std::vector <character> _buffer;
		maxim::size				_size;
	};
}

#endif // _maxim_io_character_buffer_hpp_