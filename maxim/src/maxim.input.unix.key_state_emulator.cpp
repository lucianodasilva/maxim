#include "maxim.input.unix.key_state_emulator.hpp"

namespace maxim::input::_unix {

    std::vector<key_event_args> key_state_emulator::update_key_states (std::vector < key_info > const & key_states) {
        using namespace std::chrono;

        std::vector<key_event_args> events;

        auto now = high_resolution_clock::now();

        for (auto & key : key_states) {

            key_event_args args { key, true };

            // set or reset counter for key combination
            if (key.char_value != 0 || key.key != virtual_key::none) {
                auto key_it = _key_states.find(args);
                bool is_new_key = key_it == _key_states.end();

                if (is_new_key) {
                    _key_states[args] = now;

                    events.push_back(key_event_args{
                            key.char_value,
                            key.key,
                            key.modifiers,
                            true
                    });
                } else {
                    key_it->second = now;
                }
            }
        }

        // check for time timeouts
        for (auto & state : _key_states) {
            auto time = state.second;
            auto time_span = duration_cast<milliseconds>(now - time).count();

            if (time_span > key_delay_ms) {
                auto & key = state.first.key;

                events.push_back(key_event_args{
                        {
                                key.char_value,
                                key.key,
                                key.modifiers
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