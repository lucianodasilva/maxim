#include "maxim.input.unix.key_state_emulator.hpp"

namespace maxim::input::_unix {

    std::vector<key_event_args> key_state_emulator::update_key_state(key_event_args const & args) {
        std::vector<key_event_args> events;

        auto & key = args.key;

        auto time = std::chrono::high_resolution_clock::now();
        // set or reset counter for key combination
        if (key.char_value != 0 || key.key != virtual_key::none) {
            auto key_it = _key_states.find(args);
            bool is_new_key = key_it == _key_states.end();

            if (is_new_key) {
                _key_states[args] = time;

                events.push_back(key_event_args {
                    key.char_value,
                    key.key,
                    key.modifiers,
                    true
                });
            } else {
                key_it->second = time;
            }
        }

        // check for time timeouts
        for (auto k : _key_states) {
            auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(time - k.second).count();
            if (time_span > key_delay_ms) {
                // remove
                events.push_back(key_event_args {
                    {
                        k.first.key.char_value,
                        k.first.key.key,
                        k.first.key.modifiers
                    },
                    false
                });
            }
        }

        // remove from key state cache
        for (auto &e : events) {
            if (!e.is_down)
                _key_states.erase(e);
        }

        return events;
    }

}