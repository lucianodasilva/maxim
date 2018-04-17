#include <maxim.hpp>
#include <iostream>

int main(int arg_c, char ** arg_v)
{
    using namespace maxim::tui;

    maxim::tui::device tui_device (std::make_unique <driver_xterm>());

    tui_device.fullscreen(true);
    tui_device.enable_cursor(false);

    tui_device.draw_solid_box(maxim::tui::colors::green, {5, 5}, {5, 5});
    tui_device.draw_text ("Luciano da Silva", {10, 10});

    tui_device.present();

    tui_device.draw_solid_box(maxim::tui::colors::red, {8, 8}, {5, 5});
    tui_device.draw_solid_box(maxim::tui::colors::blue, {15, 15}, {20, 20});
    tui_device.present();

    tui_device.draw_text ("Yada Yada Yada", {16, 16}, maxim::tui::colors::black, maxim::tui::colors::red,
                          {
                              false, // bold
                              false, // underlined
                              true, // blink
                              false  // reverse
                          });

    tui_device.present();

    int i;
    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);

    return 0;
}