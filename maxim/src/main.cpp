#include <maxim.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

void print_key_state (maxim::device & device, maxim::input::key_info const & args, bool down) {
    auto g = device.graphics();

    std::stringstream str;

    str << std::setfill('0');

    str
            << "C [0x" << std::hex << std::setw(2) << (uint32_t)args.char_value << "] "
            << "V [0x" << std::hex << std::setw(2) << (uint32_t)args.key << "] "
            << "M [0x" << std::hex << std::setw(2) << (uint32_t)args.modifiers << "]";

    maxim::color_t
        background = maxim::colors::black,
        foreground = maxim::colors::white;

    if (down) {
        background = maxim::colors::dark_green;
        foreground = maxim::colors::dark_gray;
    }

    g.clear();
    g.draw_text (str.str(), { 3, 3}, foreground, background);
}

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;

	application app;

	app.on_key_down += [](application & a, input::key_info const & args) {
        print_key_state (a.device(), args, true);

		if (args.key == input::virtual_key::C && args.modifiers == input::modifier_key::control) {
			a.exit();
		}
	};

    app.on_key_up += [](application & a, input::key_info const & args) {
        print_key_state (a.device(), args, false);
    };

    app.device().fullscreen(true);
    app.device().enable_cursor(false);
    app.device().clear();
	app.run();

    return 0;
}