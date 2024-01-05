#include "maxim.device.hpp"
#include "maxim.input.hpp"
#include <maxim.io.hpp>

#include <iostream>

class application : public maxim::input_event_handler {
public:

    void on_key_down (maxim::input::key_info info) override {
        using namespace maxim::input;

        if (info.key == (key::c | key::control)) {
            _running = false;
        }
    }

    void run () {
        using namespace maxim;
        using namespace std::chrono;
        using namespace std::chrono_literals;

        _device.fullscreen(true);

        auto c1 = colors::RED;
        auto c2 = colors::BLUE;
        auto theta = 0.0F;
        auto theta_step = 1.0F;

        auto frame_time = system_clock::now();
        auto frame_duration = 1s / 60;

        while (_running) {
            auto elapsed_time = system_clock::now() - frame_time;
            frame_time = system_clock::now();

            theta += theta_step * (duration_cast < milliseconds > (elapsed_time).count() / 1000.0F);

            if (theta >= 1.0F || theta <= 0.0F) {
                std::clamp(theta, 0.0F, 1.0F);
                theta_step *= -1.0F;
            }

            auto g = _device.graphics();
            g.draw_box({1, 1, 10, 10}, lerp(c1, c2, theta));

            _device.present();

            // evaluate input
            _device.dispatch_input_events(*this);

            // keep 60 fps max
            auto sleep_time = frame_duration - (system_clock::now() - frame_time);

            if (sleep_time > 0ms) {
                std::this_thread::sleep_for(sleep_time);
            }
        }
    }

private:
    maxim::device   _device;
    bool            _running = true;
};

int main(int arg_c, char ** arg_v){
    application app;
    app.run();

    return 0;
}