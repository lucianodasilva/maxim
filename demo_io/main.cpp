#include <maxim.io.hpp>
#include <thread>

class application : public maxim::input_event_handler {
public:

	void run () {
		using namespace maxim;
		_running = true;

		while (_running) {
			auto g = maxim::graphics(_device.backbuffer());
			g.clear ();

			g.draw_h_line({0, 0}, 65, colors::red);
			g.draw_v_line({0, 0}, 65, colors::green);

			g.draw_text("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", {1, 1}, colors::white);

			auto test_text = [&](std::string const & text, maxim::rect bounds, alignment h_align, alignment v_align, bool wrap = false) {
				g.draw_rect (bounds, colors::gray);
				g.draw_text(text, bounds, colors::black, {h_align, v_align, {}, wrap});
			};

			test_text ("Top Left", {2, 3, 20, 5}, alignment::near, alignment::near);
			test_text ("Top Center", {23, 3, 20, 5}, alignment::center, alignment::near);
			test_text ("Top Right", {44, 3, 20, 5}, alignment::far, alignment::near);
			test_text ("Center Left", {2, 9, 20, 5}, alignment::near, alignment::center);
			test_text ("Center Center", {23, 9, 20, 5}, alignment::center, alignment::center);
			test_text ("Center Right", {44, 9, 20, 5}, alignment::far, alignment::center);
			test_text ("Bottom Left", {2, 15, 20, 5}, alignment::near, alignment::far);
			test_text ("Bottom Center", {23, 15, 20, 5}, alignment::center, alignment::far);
			test_text ("Bottom Right", {44, 15, 20, 5}, alignment::far, alignment::far);

			test_text ("Top Left With Wrapping", {2, 21, 20, 5}, alignment::near, alignment::near, true);
			test_text ("Top Center With Wrapping", {23, 21, 20, 5}, alignment::center, alignment::near, true);
			test_text ("Top Right With Wrapping", {44, 21, 20, 5}, alignment::far, alignment::near, true);
			test_text ("Center Left With Wrapping", {2, 27, 20, 5}, alignment::near, alignment::center, true);
			test_text ("Center Center With Wrapping", {23, 27, 20, 5}, alignment::center, alignment::center, true);
			test_text ("Center Right With Wrapping", {44, 27, 20, 5}, alignment::far, alignment::center, true);
			test_text ("Bottom Left With Wrapping", {2, 33, 20, 5}, alignment::near, alignment::far, true);
			test_text ("Bottom Center With Wrapping", {23, 33, 20, 5}, alignment::center, alignment::far, true);
			test_text ("Bottom Right With Wrapping", {44, 33, 20, 5}, alignment::far, alignment::far, true);

			// transform test
			{
				graphics_transform_guard tnc_guard (g, { 80, 5, 60, 20 });

				g.draw_h_line({ 0, 0 }, 65, colors::red);
				g.draw_v_line({ 0, 0 }, 25, colors::green);

				g.draw_text("1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", { 1, 1 }, colors::white);
			}

			_device.present ();
			_device.dispatch_input_events(*this);
			std::this_thread::yield ();
		}
	}

	void stop () {
		_running = false;
	}

	void on_key_press (maxim::input::key_info const & e) override {
		if (e.matches (maxim::input::virtual_key::escape) || e.matches (maxim::input::virtual_key::Q)) {
			this->stop ();
		}
	}

private:
	maxim::device   _device;
	bool            _running = false;
};

int main(int arg_c, char ** arg_v)
{
    // initialize the application
    application app;
    app.run();

    return 0;
}