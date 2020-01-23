#include <maxim.io.hpp>
#include <thread>
#include <csignal>

class application : public maxim::input_event_handler {
public:

	application () {
		device.set_fullscreen(true);
		device.set_cursor_enabled(false);
	}

	void on_key_down (maxim::input::key_info const & key) override {
		using namespace maxim::input;

		if (key.key == virtual_key::C &&
			key.modifiers == modifier_key::control)
		{
			running = false;
		}
	}

	void render () {
		using namespace maxim;

		graphics g { device.backbuffer () };

		g.clear ();

		g.draw_h_line (
				point { 0, 5 },
				device.get_size().width,
				colors::purple);

		g.draw_box(
				{10, 10, 100, 100},
				colors::red,
				true);

		device.present ();
	}

	void run () {

		while (running) {
			device.dispatch_input_events(*this);

			render ();
			std::this_thread::yield ();
		}

	}

	maxim::device 	device;
	bool 			running {true};
};

int main(int arg_c, char ** arg_v)
{

	application app;
	app.run ();

    return 0;
}