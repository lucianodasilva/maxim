#include "maxim.input.unix.key_state_emulator.hpp"
#include "maxim.device.hpp"

namespace maxim::input::unix {

    void key_state_emulator::timeout_and_dispatch(maxim::input_event_handler & handler) {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();

        // run through all keys and remove them + raise key up event, if they are timed out
        for (auto it = _pressed_keys.begin(); it != _pressed_keys.end();) {
            auto time = it->second;
            auto time_span = duration_cast<milliseconds>(now - time).count();

            if (time_span > KEY_DELAY_MS) {
                // on timeout dispatch and erase from state
                auto info = it->first;
                handler.on_key_up (info);

                it = _pressed_keys.erase(it);
            } else {
                ++it;
            }
        }
    }

    void key_state_emulator::evaluate_and_dispatch(maxim::input::key_info info, maxim::input_event_handler & handler) {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();

        // set or reset counter for key combination
        if (info) {
            auto key_it = _pressed_keys.find(info);

            if (key_it == _pressed_keys.end()) {
                _pressed_keys[info] = now;

                // dispatch key down event
                handler.on_key_down (info);
            } else {
                key_it->second = now;
            }
        }
    }

}