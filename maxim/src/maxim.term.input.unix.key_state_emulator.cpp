#include "maxim.term.input.unix.key_state_emulator.hpp"

namespace maxim {
    namespace term {
        namespace input {
            namespace _unix {

                std::vector<input_event_info> key_state_emulator::update_key_state(key_event_args const &key) {
                    std::vector<input_event_info> events;

                    auto time = std::chrono::high_resolution_clock::now();
                    // set or reset counter for key combination
                    if (key.char_value != 0) {
                        auto key_it = _key_states.find(key);
                        bool is_new_key = key_it == _key_states.end();

                        if (is_new_key) {
                            _key_states[key] = time;

                            events.push_back(input_event_info{
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
                            events.push_back(input_event_info{
                                k.first.char_value,
                                k.first.key,
                                k.first.modifiers,
                                false
                            });
                        }
                    }

                    // remove from key state cache
                    for (auto &e : events) {
                        if (!e.is_down)
                            _key_states.erase({
                                                  e.char_value,
                                                  e.key,
                                                  e.modifiers});
                    }

                    return events;
                }

            }
        }
    }
}