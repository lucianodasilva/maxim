//#include <maxim.hpp>
#include <iostream>

#include <termio.h>
#include <termios.h>

#include <unistd.h>
#include <fcntl.h>

#include <cinttypes>
#include <cstring>

//#include <maxim.tui.input.hpp>

/*
struct key_map_item {
    virtual_key	key;
    alt_key     alt;
};

key_map_item key_map [] = {
    {}, // 0x00
    {virtual_key::A, alt_key::control}, // 0x01
    {virtual_key::B, alt_key::control}, // 0x02
    {virtual_key::C, alt_key::control}, // 0x03
    {virtual_key::D, alt_key::control}, // 0x04
    {virtual_key::E, alt_key::control}, // 0x05
    {virtual_key::F, alt_key::control}, // 0x06
    {virtual_key::G, alt_key::control}, // 0x07
    {virtual_key::backspace, alt_key::control}, // 0x08
    {virtual_key::tab, alt_key::none }, // 0x09
    {virtual_key::J, alt_key::control}, // 0x0A
    {virtual_key::K, alt_key::control}, // 0x0B
    {virtual_key::L, alt_key::control}, // 0x0C
    {virtual_key::enter, alt_key::control}, // 0x0D
    {virtual_key::N, alt_key::control}, // 0x0E
    {virtual_key::O, alt_key::control}, // 0x0F
    {virtual_key::P, alt_key::control}, // 0x10
    {virtual_key::Q, alt_key::control}, // 0x11
    {virtual_key::R, alt_key::control}, // 0x12
    {virtual_key::S, alt_key::control}, // 0x13
    {virtual_key::T, alt_key::control}, // 0x14
    {virtual_key::U, alt_key::control}, // 0x15
    {virtual_key::V, alt_key::control}, // 0x16
    {virtual_key::W, alt_key::control}, // 0x17
    {virtual_key::X, alt_key::control}, // 0x18
    {virtual_key::Y, alt_key::control}, // 0x19
    {virtual_key::Z, alt_key::control}, // 0x1A
    {virtual_key::escape, alt_key::none}, // 0x1B
    {}, // 0x1C
    {}, // 0x1D
    {}, // 0x1E
    {}, // 0x1F
    {virtual_key::space, alt_key::none}, // 0x20
    {virtual_key::none, alt_key::shift}, // 0x21
    {virtual_key::none, alt_key::shift}, // 0x22
    {virtual_key::none, alt_key::shift}, // 0x23
    {virtual_key::none, alt_key::shift}, // 0x24
    {virtual_key::none, alt_key::shift}, // 0x25
    {virtual_key::none, alt_key::shift}, // 0x26
    {virtual_key::none, alt_key::shift}, // 0x27
    {virtual_key::none, alt_key::shift}, // 0x28
    {virtual_key::none, alt_key::shift}, // 0x29
    {}, // 0x2A
    {}, // 0x2B
    {}, // 0x2C
    {}, // 0x2D
    {}, // 0x2E
    {}, // 0x2F
    {}, // 0x30
    {}, // 0x31
    {}, // 0x32
    {}, // 0x33
    {}, // 0x34
    {}, // 0x35
    {}, // 0x36
    {}, // 0x37
    {}, // 0x38
    {}, // 0x39
    {}, // 0x3A
    {}, // 0x3B
    {}, // 0x3C
    {virtual_key::none, alt_key::shift}, // 0x3D
    {}, // 0x3E
    {}, // 0x3F
    {}, // 0x40
    {virtual_key::A, alt_key::shift}, // 0x41
    {virtual_key::B, alt_key::shift}, // 0x42
    {virtual_key::C, alt_key::shift}, // 0x43
    {virtual_key::D, alt_key::shift}, // 0x44
    {virtual_key::E, alt_key::shift}, // 0x45
    {virtual_key::F, alt_key::shift}, // 0x46
    {virtual_key::G, alt_key::shift}, // 0x47
    {virtual_key::H, alt_key::shift}, // 0x48
    {virtual_key::I, alt_key::shift}, // 0x49
    {virtual_key::J, alt_key::shift}, // 0x4A
    {virtual_key::K, alt_key::shift}, // 0x4B
    {virtual_key::L, alt_key::shift}, // 0x4C
    {virtual_key::M, alt_key::shift}, // 0x4D
    {virtual_key::N, alt_key::shift}, // 0x4E
    {virtual_key::O, alt_key::shift}, // 0x4F
    {virtual_key::P, alt_key::shift}, // 0x50
    {virtual_key::Q, alt_key::shift}, // 0x51
    {virtual_key::R, alt_key::shift}, // 0x52
    {virtual_key::S, alt_key::shift}, // 0x53
    {virtual_key::T, alt_key::shift}, // 0x54
    {virtual_key::U, alt_key::shift}, // 0x55
    {virtual_key::V, alt_key::shift}, // 0x56
    {virtual_key::W, alt_key::shift}, // 0x57
    {virtual_key::X, alt_key::shift}, // 0x58
    {virtual_key::Y, alt_key::shift}, // 0x59
    {virtual_key::Z, alt_key::shift}, // 0x5A
    {}, // 0x5B
    {}, // 0x5C
    {}, // 0x5D
    {}, // 0x5E
    {}, // 0x5F
    {}, // 0x60
    {virtual_key::A, alt_key::none}, // 0x61
    {virtual_key::B, alt_key::none}, // 0x62
    {virtual_key::C, alt_key::none}, // 0x63
    {virtual_key::D, alt_key::none}, // 0x64
    {virtual_key::E, alt_key::none}, // 0x65
    {virtual_key::F, alt_key::none}, // 0x66
    {virtual_key::G, alt_key::none}, // 0x67
    {virtual_key::H, alt_key::none}, // 0x68
    {virtual_key::I, alt_key::none}, // 0x69
    {virtual_key::J, alt_key::none}, // 0x6A
    {virtual_key::K, alt_key::none}, // 0x6B
    {virtual_key::L, alt_key::none}, // 0x6C
    {virtual_key::M, alt_key::none}, // 0x6D
    {virtual_key::N, alt_key::none}, // 0x6E
    {virtual_key::O, alt_key::none}, // 0x6F
    {virtual_key::P, alt_key::none}, // 0x70
    {virtual_key::Q, alt_key::none}, // 0x71
    {virtual_key::R, alt_key::none}, // 0x72
    {virtual_key::S, alt_key::none}, // 0x73
    {virtual_key::T, alt_key::none}, // 0x74
    {virtual_key::U, alt_key::none}, // 0x75
    {virtual_key::V, alt_key::none}, // 0x76
    {virtual_key::W, alt_key::none}, // 0x77
    {virtual_key::X, alt_key::none}, // 0x78
    {virtual_key::Y, alt_key::none}, // 0x79
    {virtual_key::Z, alt_key::none}, // 0x7A
    {}, // 0x7B
    {}, // 0x7C
    {}, // 0x7D
    {}, // 0x7E
    {virtual_key::backspace, alt_key::none} // 0x7F
};

struct key {
    char        char_value;
    virtual_key command;
    alt_key     alt;
};

void key_to_string (key k) {
    std::cout
        << "'" << k.char_value << "'"
        << " c: " << std::hex << (uint8_t)k.command
        << " a:" << std::hex << (uint8_t)k.alt << std::endl;
}

*/
/*
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
*/

/*
virtual_key map_key_code(uint16_t key_code) {
	switch (key_code) {
	case 32:
		return virtual_key::insert;
	case 33:
		return virtual_key::del;
	case 35:
		return virtual_key::page_up;
	case 36:
		return virtual_key::page_down;
	case 41:
		return virtual_key::up;
	case 42:
		return virtual_key::down;
	case 43:
		return virtual_key::right;
	case 44:
		return virtual_key::left;
	case 46:
		return virtual_key::end;
	case 48:
		return virtual_key::home;
	case 50:
		return virtual_key::f1;
	case 51:
		return virtual_key::f2;
	case 52:
		return virtual_key::f3;
	case 53:
		return virtual_key::f4;
	case 3135:
		return virtual_key::f5;
	case 3137:
		return virtual_key::f6;
	case 3138:
		return virtual_key::f7;
	case 3139:
		return virtual_key::f8;
	case 3230:
		return virtual_key::f9;
	case 3231:
		return virtual_key::f10;
	case 3233:
		return virtual_key::f11;
	case 3234:
		return virtual_key::f12;
	default:
		return virtual_key::none;
	}
}

alt_key map_modifiers(uint8_t m) {
	switch (m) {
	case 32:
		return alt_key::shift;
	case 33:
		return alt_key::alt;
	case 34:
		return alt_key::shift | alt_key::alt;
	case 35:
		return alt_key::control;
	case 36:
		return alt_key::shift | alt_key::control;
	case 37:
		return alt_key::control | alt_key::alt;
	case 38:
		return alt_key::control | alt_key::alt | alt_key::shift;
	default:
		return alt_key::none;
	}
}

*/

template < std::size_t len >
inline bool match (uint8_t * buffer, char const (&lit)[len]) noexcept {
    return std::equal(buffer, buffer + len, lit);
}

/*

key parse_input () {
    uint8_t buffer [sizeof(uint64_t)] = {};
    int r = 0;

    r = read(STDIN_FILENO, +buffer, sizeof(buffer));

    if (r == 0)
        return {};

	// regular keys
    if (r == 1 && buffer[0] < 0x80) {
        return {
            *reinterpret_cast < char *> (+buffer),
            key_map [buffer[0]].key,
            key_map [buffer[0]].alt
        };
	}
	else {
		// regular keys + alt
		if (r == 2 && buffer[0] == '\x1B') {
			return {
				*reinterpret_cast <char *> (1 + buffer),
				key_map[buffer[1]].key,
				key_map[buffer[1]].alt | alt_key::alt
			};
		} else if (buffer[0] == 0x1B) {
			uint16_t key_code = 0;
			alt_key alt = alt_key::none;

			// simple special keys
			if (r < 5) {
				key_code = buffer[2];
			} else if (r == 5) {
				key_code = *reinterpret_cast <uint16_t *> (buffer + 2);
			}
			// special key with modifier combinations
			else if (r == 6) {
				if (buffer[5] == 0x7e) {
					key_code = buffer[2];
				} else {
					key_code = buffer[5];
				}
				alt = map_modifiers(buffer[4]);
			}
			else if (r == 7) {
				key_code = *reinterpret_cast <uint16_t *> (buffer + 2);
				alt = map_modifiers(buffer[6]);
			}

			return { 0x00, map_key_code(key_code), alt };
		}
    }
}

*/

int main(int arg_c, char ** arg_v)
{
	//using namespace maxim::tui;

	termios new_settings;  // NOLINT

	termios initial_settings{};
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

	new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
	new_settings.c_cflag
		&= ~(OPOST);
	new_settings.c_cflag |= CS8;
	new_settings.c_cc[VMIN] = 0;
	new_settings.c_cc[VTIME] = 1;

	tcsetattr(0, TCSANOW, &new_settings);
	fcntl(STDIN_FILENO, F_SETFL, FNDELAY);

    //while (i = getchar()){
	bool run = true;
	while(run) {
	    /*
	    auto v = parse_input();

	    if (v.char_value == 0)
	        continue;

	    key_to_string(v);

	    if (v.alt == alt_key::control && v.char_value == 'c')
	        break;
        */

		int r;
        uint8_t buffer [8];

		while ((r = read(STDIN_FILENO, +buffer, 8)) != EOF) {
			memset(+buffer, 0, 8);

			std::cout << std::dec;
			//std::cout << "r: " << r << " str: " << reinterpret_cast < char * > (+buffer) << std::endl;
			std::cout << "r: " << r << " | ";
			std::cout << std::hex;
			std::cout << "hex: ";
			for (auto i : buffer)
			{
				std::cout << " ";

				std::cout.fill('0');
				std::cout.width(2);
				std::cout << static_cast<int>(i);
			}
			std::cout << std::endl;

			if (buffer[0] == 3)
				run = false;
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

	/*
	application app;
	
	app.on_start += [](application &) {
		std::cout << "app started" << std::endl;
	};

	app.on_key_down += [](application & a, key_event_args const & args) {
		std::cout << "keypress: '" << args.char_value << "' virtual key: " << std::hex << (uint32_t)args.key << std::endl;

		//if (args.key == (virtual_key::control | virtual_key::C))
		//	a.exit();
	};

	app.run();
	*/

	/*
    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);
	*/

    //return 0;
}