#include <maxim.hpp>
#include <iostream>

#include <termio.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

void draw_grid(maxim::tui::device & device, maxim::tui::rect const & bounds) {
	using namespace maxim::tui;

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

int main(int arg_c, char ** arg_v)
{
    using namespace maxim::tui;

	struct termios new_settings;

	termios initial_settings;
	tcgetattr(0,&initial_settings);

	new_settings = initial_settings;

	/*
	new_settings.c_lflag &= ~ICANON;
	new_settings.c_lflag &= ~IEXTEN;
    new_settings.c_lflag &= ~ECHO;
	new_settings.c_lflag &= ~ISIG;

	new_settings.c_iflag &= ~IXON;
	new_settings.c_iflag &= ~IUTF8;

	for (int i = 0; i < 32; ++i) {
	    new_settings.c_cc[i] = 0;
	}
	*/

	new_settings.c_iflag |= IGNBRK;
	new_settings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
	new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &new_settings);
	fcntl(STDIN_FILENO, F_SETFL, FNDELAY);

    //while (i = getchar()){
	while(true) {
		int i = 0;
		int r = 0;

		r = read(STDIN_FILENO, &i, 4);

		if (r != EOF) {
            std::cout << "r: " << std::dec << r << " c:" << std::hex << i << std::endl;

            if (i == 3)
                return 0;
        }
    }

	fcntl(STDIN_FILENO, 0, FNDELAY);
	tcsetattr(0, TCSANOW, &initial_settings);

    return 0;

	/*
    device tui_device;

    tui_device.fullscreen(true);
    tui_device.enable_cursor(false);

	rect view = { {30, 10}, { 15, 15 }};

	draw_grid(tui_device, view);

    auto g = tui_device.graphics();

	g.view = view;

    g.draw_text("Luciano André Melo da Silva", {25, 2}, colors::gray);

    g.draw_box({{2, 2}, {20, 5}}, colors::yellow, false);
    g.draw_box({{10, 10}, {25, 10}}, colors::green, false);

    g.draw_box({{40, 5}, {30, 15}}, colors::pink);

    tui_device.present();
	*/
	application app;
	
	app.on_start += [](application &) {
		std::cout << "app started" << std::endl;
	};

	app.on_key_down += [](application & a, key_event_args const & args) {
		std::cout << "keypress: '" << args.char_value << "' virtual key: " << std::hex << (uint32_t)args.key << std::endl;

		if (args.key == (virtual_key::control | virtual_key::C))
			a.exit();
	};

	app.run();

	/*
    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);
	*/

    return 0;
}