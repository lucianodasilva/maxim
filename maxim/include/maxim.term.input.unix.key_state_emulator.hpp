#pragma once
#ifndef _maxim_term_input_unix_key_state_emulator_hpp_
#define _maxim_term_input_unix_key_state_emulator_hpp_

#include "maxim.term.input.hpp"

#include <chrono>
#include <unordered_map>

namespace maxim {
    namespace term {
        namespace input {
            namespace _unix {

                struct key_state_emulator {
                public:

                    static constexpr uint32_t key_delay_ms = 150;
                    static constexpr uint32_t key_delay_ds = key_delay_ms / 100;

                    std::vector<input_event_info> update_key_state(key_event_args const &key);

                private:
                    // key press state
                    std::unordered_map<
                        key_event_args,
                        std::chrono::high_resolution_clock::time_point
                    >
                        _key_states;
                };

            }
        }
    }
}

#endif
