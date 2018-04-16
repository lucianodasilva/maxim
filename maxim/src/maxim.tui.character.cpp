#include "maxim.tui.character.hpp"

namespace maxim {
    namespace tui {

        std::vector < character_span > character_buffer::calculate_changes (
                character_buffer const & buffer
        ) const {

            if (_size != buffer._size)
                throw std::runtime_error ("changes between different sized buffers not supported");

            std::vector < character_span > changes;

            for (uint32_t y = 0; y < _size.y; ++y) {
                uint32_t
                        line_index = _size.x * y,
                        begin_index = line_index,
                        ending_index = begin_index;

                for (uint32_t x = 0; x < _size.x; ++x) {

                    bool is_same = at(line_index + x) == buffer.at(line_index + x);

                    if (begin_index == ending_index) {
                        ++ending_index;

                        if (is_same) {
                            ++begin_index;
                        }
                    } else {
                        ++ending_index;

                        if (is_same) {
                            changes.push_back({begin_index, ending_index});
                            begin_index = ending_index;
                        }
                    }

                    if (begin_index != ending_index) {
                        changes.push_back({begin_index, ending_index});
                    }
                }
            }

            return changes;
        }

    }
}