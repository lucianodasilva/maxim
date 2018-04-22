#include <maxim.hpp>
#include <iostream>

int main(int arg_c, char ** arg_v)
{
    using namespace maxim::tui;

    int i;

    device tui_device (std::make_unique <driver_xterm>());

    tui_device.fullscreen(true);
    tui_device.enable_cursor(false);

    tui_device.draw_box({10,10}, {25, 10}, colors::yellow, false);
    tui_device.draw_h_line({10, 35}, 30, colors::dark_red, false);
    tui_device.present();

    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);

    return 0;
}