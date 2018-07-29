#pragma once
#ifndef _MAXIM_INPUT_UNIX_KEY_STATE_EMULATOR_HPP_
#define _MAXIM_INPUT_UNIX_KEY_STATE_EMULATOR_HPP_

#include "maxim.input.hpp"
#include <unordered_map>

namespace maxim {
    namespace input {
        namespace _unix {

            struct key_state_emulator {
            public:

                static constexpr uint32_t key_delay_ms = 150;
                static constexpr uint32_t key_delay_ds = key_delay_ms / 100;

                std::vector < input_event_info > update_key_state (key_event_args const & key);

            private:
                // key press state
                std::unordered_map <
                    key_event_args,
                    std::chrono::high_resolution_clock::time_point
                >
                    _key_states;
            };

        }
    }
}

#endif // _MAXIM_INPUT_UNIX_KEY_STATE_EMULATOR_HPP_
