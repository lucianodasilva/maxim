#include "maxim.device.hpp"
#include "maxim.input.hpp"
#include <maxim.io.hpp>

class application : public maxim::input_event_handler {
public:

    void on_key_down (maxim::input::key_info info) override {
        using namespace maxim::input;

        if (info.key == key::escape) {
            _running = false;
        }
    }

    void run () {
        while (_running) {
            // render
            _device.clear ();

            _device.present();

            // evaluate input
            _device.dispatch_input_events(*this);
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