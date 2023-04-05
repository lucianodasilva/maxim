#pragma once
#ifndef MAXIM_INPUT_UNIX_KEY_STATE_EMULATOR_HPP
#define MAXIM_INPUT_UNIX_KEY_STATE_EMULATOR_HPP

#include "maxim.input.hpp"

#include <chrono>
#include <unordered_map>

namespace maxim {

    class input_event_handler;

    namespace input::unix {

        struct key_state_emulator {
        public:

            static constexpr uint32_t KEY_DELAY_MS = 150;
            static constexpr uint32_t KEY_DELAY_DS = KEY_DELAY_MS / 100;

            void timeout_and_dispatch  (input_event_handler & handler);

            void evaluate_and_dispatch (key_info info, input_event_handler & handler);

        private:
            // key press state
            std::unordered_map<
                key_info,
                std::chrono::high_resolution_clock::time_point
            >
                _pressed_keys;
        };

    }
}

#endif
