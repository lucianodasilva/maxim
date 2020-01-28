#include <maxim.io.hpp>
#include <maxim.ui.hpp>

#include <cmath>
#include <chrono>
#include <thread>

#include <memory>

namespace styles {

	maxim::color_t blue { maxim::colors::rgb_to_x256(135,175, 255) };
	maxim::color_t purple { maxim::colors::rgb_to_x256(91,44, 125) };
	maxim::color_t pink { maxim::colors::rgb_to_x256(232,34, 72) };

	maxim::color_t dark_dark_gray { maxim::colors::rgb_to_x256 (75, 75, 75)};

	maxim::style_t purple_text = maxim::make_style ()
			.backcolor(blue)
			.forecolor(purple);
}

/*
struct progress_bar {
public:

	void render (maxim::graphics & g) {
		auto const original_view = g.view;

		auto value_width = static_cast < int > (rint (width * (value / static_cast < float > (max - min))));

		g.draw_box ({position, {width, 1}}, normal.backcolor, true);
		g.draw_box ( {position, { value_width, 1 }}, highlighted.backcolor, true);

		g.view = { position, { value_width, 1 }};
		g.draw_text(text, position, width, text_alignment, highlighted);

		g.view = { position + maxim::point { value_width, 0 }, { width - value_width, 1 }};
		g.draw_text(text, position, width, text_alignment, normal);

		g.view = original_view;
	}

	int min {0},
		max {0},
		value {0};

	maxim::style_t 		normal {},
						highlighted {};

	maxim::point 		position {};
	int 				width {0};
	std::string 		text;
	maxim::alignment	text_alignment { maxim::alignment::centered };
};

class panel {
public:



};

class application : public maxim::input_event_handler {
public:

	application () {
		device.set_fullscreen(true);
		device.set_cursor_enabled(false);

		bar.position = { 10, 10 };
		bar.width = 50;

		bar.min = 0;
		bar.max = 10000;

		bar.normal.backcolor = maxim::colors::black;
		bar.normal.forecolor = maxim::colors::white;
		bar.highlighted.backcolor = styles::pink;
		bar.highlighted.forecolor = maxim::colors::black;
	}

	void on_key_down (maxim::input::key_info const & key) override {
		using namespace maxim::input;

		if (key.key == virtual_key::X)
		{
			running = false;
		}
	}

	void render () {
		using namespace maxim;
		graphics g { device.backbuffer () };
		auto size = device.get_size ();

		g.clear ();

		// top
		g.draw_box (
				{ 0, 0, size.width, 5 },
				styles::blue,
				true);


		// bottom
		g.draw_box (
				{0, size.height - 5, size.width, 5 },
				styles::dark_dark_gray,
				true);

		bar.position = { 8, size.height - 3 };
		bar.width = size.width - 16;
		bar.render (g);

		g.draw_text ("THIS MACHINE WILL BE BRANDED IN 10 SECONDS", { 8, size.height / 2 - 2 }, size.width - 16, maxim::alignment::centered, styles::purple_text );

		device.present ();
	}

	void run () {

		while (running) {
			device.dispatch_input_events(*this);

			render ();
			std::this_thread::yield ();

			// update animation
			using namespace std::chrono;

			auto duration = duration_cast < milliseconds > (high_resolution_clock::now ().time_since_epoch()).count () % 10000;

			bar.value = duration;
			bar.text = std::to_string (static_cast < int > (10 - ((bar.value / 10000.0F) * 10.0F)) + 1);
		}
	}

	maxim::device 	device;
	bool 			running {true};

	maxim::style_t	text_style;

	progress_bar	bar;
};
*/

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;

	application app;
	auto & device = app.device ();

	device.set_fullscreen(true);
	device.set_cursor_enabled (false);

	// setup controls
	grid root;

	root.columns = { 10.0_var, 10.0_var, 10.0_var, 10.0_var };
	root.rows = { 5.0_const, 1.0_var };

	auto * l1 = root.create_element < label > ();

	l1->columns = grid::range (0, 1);
	l1->rows = grid::range(0, 1);
	l1->text = "--- [1] ---";
	l1->style = make_style()
		.backcolor(colors::cyan)
		.forecolor(colors::black);

	auto * l2 = root.create_element < label > ();

	l2->columns = grid::range (2);
	l2->rows = grid::range(0);
	l2->text = "--- [2] ---";
	l2->style = make_style()
		.backcolor(colors::red)
		.forecolor(colors::green);

	auto * l3 = root.create_element < label > ();

	l3->columns = grid::range (3);
	l3->rows = grid::range(0, 1);
	l3->text = "--- [3] ---";
	l3->style = make_style()
		.backcolor(colors::yellow)
		.forecolor(colors::pink);

	// set as active
	app.root = &root;

	app.on_key_down += [](application & sender, input::key_info e) {
		if (e.key == input::virtual_key::Q)
			sender.exit ();
	};

	app.run();


    return 0;
}