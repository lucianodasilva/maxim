#include "maxim.input.unix.key_state_emulator.hpp"
#include "maxim.application.hpp"

namespace maxim::input::_unix {

    void key_state_emulator::timeout_and_dispatch(maxim::application &app) {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();

        auto it = _key_states.begin();

        while (it != _key_states.end()) {
            auto time = it->second;
            auto time_span = duration_cast<milliseconds>(now - time).count();

            if (time_span > key_delay_ms) {
                // on timeout dispatch and erase from state
                auto key = it->first;
                it = _key_states.erase (it);

                app.on_key_up.invoke (app, key);
            } else {
                ++it;
            }
        }
    }

    void key_state_emulator::evaluate_and_dispatch(maxim::input::key_info key, maxim::application &app) {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();

        // set or reset counter for key combination
        if (key.char_value != 0 || key.key != virtual_key::none) {

            auto key_it = _key_states.find(key);

            if (key_it == _key_states.end()) {
                _key_states[key] = now;

                // dispatch
                app.on_key_down.invoke (app, key);
            } else {
                key_it->second = now;
            }
        }
    }

}