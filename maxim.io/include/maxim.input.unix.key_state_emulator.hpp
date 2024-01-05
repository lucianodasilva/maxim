#pragma once
#ifndef _maxim_input_unix_key_state_emulator_hpp_
#define _maxim_input_unix_key_state_emulator_hpp_

#include "maxim.input.hpp"

#include <chrono>
#include <unordered_map>

namespace maxim {

    class input_event_handler;

    namespace input::_unix {

        struct key_state_emulator {
        public:

            static constexpr uint32_t key_delay_ms = 150;
            static constexpr uint32_t key_delay_ds = key_delay_ms / 100;

            void timeout_and_dispatch  (input_event_handler & handler);

            void evaluate_and_dispatch (key_info key, input_event_handler & handler);

        private:
            // key press state
            std::unordered_map<
                key_info,
                std::chrono::system_clock::time_point
            >
                _key_states;
        };

    }
}

#endif
