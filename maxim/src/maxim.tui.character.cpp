#include "maxim.tui.character.hpp"

namespace maxim {
    namespace tui {

        character_change_set character_buffer::calculate_change_set (
                character_buffer const & buffer
        ) const {

            if (_size != buffer._size)
                throw std::runtime_error ("changes between different sized buffers not supported");

            character_change_set changes;

            for (uint32_t y = 0; y < _size.y; ++y) {
                uint32_t
                        line_index = _size.x * y,
                        begin_index = line_index,
                        ending_index = begin_index;

                bool has_changes = false;

                for (uint32_t x = 0; x < _size.x; ++x) {
                    ending_index = line_index + x;

                    bool is_same = at(ending_index) == buffer.at(ending_index);

                    if (!has_changes && is_same) {
                        begin_index = ending_index;
                    } else {
                        has_changes = true;

                        if (is_same) {
                            changes.push_back({begin_index + 1, ending_index});
                            begin_index = ending_index;
                            has_changes = false;
                        }
                    }
                }

                if (has_changes)
                    changes.push_back({begin_index + 1, ending_index + 1});
            }

            return changes;
        }

    }
}