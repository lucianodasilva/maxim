#include <maxim.hpp>
#include <iostream>

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

    int i;

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

	application app;
	
	app.on_start += [](application &) {
		std::cout << "app started" << std::endl;
	};

	app.run();

    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);

    return 0;
}