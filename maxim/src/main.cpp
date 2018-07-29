#include <maxim.hpp>
#include <iostream>

#include <termio.h>
#include <termios.h>

#include <unistd.h>
#include <fcntl.h>

#include <cinttypes>
#include <cstring>

/*
void draw_grid(maxim::term::device & device, maxim::term::rect const & bounds) {
	using namespace maxim::term;

	device.graphics().draw_h_line(bounds.position, bounds.size.width, colors::dark_gray);
	device.graphics().draw_h_line({bounds.left(), bounds.bottom()}, bounds.size.width, colors::dark_gray);

	device.graphics().draw_v_line(bounds.position, bounds.size.height, colors::dark_gray);
	device.graphics().draw_v_line({ bounds.right(), bounds.top() }, bounds.size.height, colors::dark_gray);

	character c = {};
	c.backcolor = colors::dark_gray;
	c.forecolor = colors::dark_gray;
	c.content = ' ';

	for (int i = 1; i < 100; i += 2) {
		device.backbuffer().at({ i, 1 }) = c;
		device.backbuffer().at({ 1, i }) = c;
	}
}
*/

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;
	using namespace maxim::term;

	/*
    device tui_device;

    tui_device.fullscreen(true);
    tui_device.enable_cursor(false);

	rect view = { {30, 10}, { 15, 15 }};

	draw_grid(tui_device, view);

    auto g = tui_device.graphics();

	g.view = view;
m
    g.draw_text("Luciano André Melo da Silva", {25, 2}, colors::gray);

    g.draw_box({{2, 2}, {20, 5}}, colors::yellow, false);
    g.draw_box({{10, 10}, {25, 10}}, colors::green, false);

    g.draw_box({{40, 5}, {30, 15}}, colors::pink);

    tui_device.present();
	*/
	application app;
	bool		can_cancel = false;
	
	app.on_start += [](application &) {
		std::cout << "app started" << std::endl;
	};

	app.on_exit += [&](application &, bool & cancel) {
		cancel = can_cancel;
	};

	app.on_key_down += [&](application & a, input::key_event_args const & args) {
		std::cout
            << "keypress: '" << args.char_value
            << "' virtual key: " << std::hex << (uint32_t)args.key
            << "' modifier key: " << std::hex << (uint32_t)args.modifiers
            << std::endl;

		if (args.key == input::virtual_key::C && args.modifiers == input::modifier_key::control) {
			std::cout << "exit called" << std::endl;
			a.exit();
		}

		if (args.key == input::virtual_key::T)
			can_cancel = !can_cancel;
	};

	app.run();

    return 0;
}