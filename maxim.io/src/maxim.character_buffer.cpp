#include "maxim.character_buffer.hpp"

#include <stdexcept>

namespace maxim {

	character_change_set character_buffer::calculate_change_set (
			character_buffer const & buffer
	) const {

		if (_size != buffer._size)
			throw std::runtime_error ("changes between different sized buffers not supported");

		character_change_set 	changes;

		for (std::size_t y = 0; y < _size.height; ++y) {
			std::size_t
					line_start = y * _size.width,
					line_end = line_start + _size.width;

			for (std::size_t x = 0; x < _size.width; ++x) {
				std::size_t index = line_start + x;

				// check if line is the same
				if (*ptr_at(index) != *buffer.ptr_at(index)) {

					// look ahead to find end of change span
					std::size_t cs_index = index;

					for (;
						cs_index < line_end && *ptr_at(index) != *buffer.ptr_at(index);
						++cs_index) {}

					changes.push_back({index, cs_index});

					// jump ahead
					x += cs_index - index;
				}
			}
		}

		return changes;
	}

}