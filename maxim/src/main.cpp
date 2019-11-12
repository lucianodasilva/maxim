#include <maxim.hpp>
#include <iostream>

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;

	application app;
	bool		can_cancel = false;
	
	app.on_start += [](application &) {
		std::cout << "app started" << std::endl;
	};

	app.on_exit += [&](application &, bool & cancel) {
		cancel = can_cancel;
	};

	app.on_key_down += [&](application & a, input::key_info const & args) {

		std::cout
            << "keypress: '" << args.char_value
            << "' virtual key: " << std::hex << (uint32_t)args.key
            << "' modifier key: " << std::hex << (uint32_t)args.modifiers
            << std::endl;

		if (args.key == input::virtual_key::C && args.modifiers == input::modifier_key::control) {
			std::cout << "exit called" << std::endl;
			a.exit();
		}

		if (args.key == input::virtual_key::T) {
            std::cout << "cancel toggle" << std::endl;
            can_cancel = !can_cancel;
        }
	};

	app.run();

    return 0;
}