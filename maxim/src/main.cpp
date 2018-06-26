#include <maxim.hpp>
#include <iostream>

#include <termio.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

//#include <maxim.tui.input.hpp>

using namespace maxim::tui;

enum class v_key : uint8_t {
    none        = 0x00,
    backspace   = 0x08,
    tab         = 0x09,

    clear       = 0x0C,
    enter       = 0x0D,

    escape      = 0x1B,
    space       = 0x20,

    page_up     = 0x21,
    page_down   = 0x22,

    end         = 0x23,
    home        = 0x24,

    left        = 0x25,
    up          = 0x26,
    right       = 0x27,
    down        = 0x28,

    insert      = 0x2D,
    del         = 0x2E,

    A           = 0x41,
    B           = 0x42,
    C           = 0x43,
    D           = 0x44,
    E           = 0x45,
    F           = 0x46,
    G           = 0x47,
    H           = 0x48,
    I           = 0x49,
    J           = 0x4A,
    K           = 0x4B,
    L           = 0x4C,
    M           = 0x4D,
    N           = 0x4E,
    O           = 0x4F,
    P           = 0x50,
    Q           = 0x51,
    R           = 0x52,
    S           = 0x53,
    T           = 0x54,
    U           = 0x55,
    V           = 0x56,
    W           = 0x57,
    X           = 0x58,
    Y           = 0x59,
    Z           = 0x5A,

    f1			= 0x70,
    f2			= 0x71,
    f3			= 0x72,
    f4			= 0x73,
    f5			= 0x74,
    f6			= 0x75,
    f7			= 0x76,
    f8			= 0x77,
    f9			= 0x78,
    f10			= 0x79,
    f11			= 0x80,
    f12			= 0x81
};

enum class alt_key {
    none        = 0x00,
    shift       = 0x01,
    control     = 0x02,
    alt         = 0x04
};

alt_key operator | (alt_key v1, alt_key v2) {
    return static_cast < alt_key > (static_cast< uint8_t > (v1) | static_cast < uint8_t > (v2));
}

struct key_map_item {
    v_key control;
    alt_key     alt;
};

key_map_item key_map [] = {
    {}, // 0x00
    {v_key::A, alt_key::control}, // 0x01
    {v_key::B, alt_key::control}, // 0x02
    {v_key::C, alt_key::control}, // 0x03
    {v_key::D, alt_key::control}, // 0x04
    {v_key::E, alt_key::control}, // 0x05
    {v_key::F, alt_key::control}, // 0x06
    {v_key::G, alt_key::control}, // 0x07
    {v_key::backspace, alt_key::control}, // 0x08
    {v_key::tab, alt_key::none }, // 0x09
    {v_key::J, alt_key::control}, // 0x0A
    {v_key::K, alt_key::control}, // 0x0B
    {v_key::L, alt_key::control}, // 0x0C
    {v_key::enter, alt_key::control}, // 0x0D
    {v_key::N, alt_key::control}, // 0x0E
    {v_key::O, alt_key::control}, // 0x0F
    {v_key::P, alt_key::control}, // 0x10
    {v_key::Q, alt_key::control}, // 0x11
    {v_key::R, alt_key::control}, // 0x12
    {v_key::S, alt_key::control}, // 0x13
    {v_key::T, alt_key::control}, // 0x14
    {v_key::U, alt_key::control}, // 0x15
    {v_key::V, alt_key::control}, // 0x16
    {v_key::W, alt_key::control}, // 0x17
    {v_key::X, alt_key::control}, // 0x18
    {v_key::Y, alt_key::control}, // 0x19
    {v_key::Z, alt_key::control}, // 0x1A
    {v_key::escape, alt_key::none}, // 0x1B
    {}, // 0x1C
    {}, // 0x1D
    {}, // 0x1E
    {}, // 0x1F
    {v_key::space, alt_key::none}, // 0x20
    {v_key::none, alt_key::shift}, // 0x21
    {v_key::none, alt_key::shift}, // 0x22
    {v_key::none, alt_key::shift}, // 0x23
    {v_key::none, alt_key::shift}, // 0x24
    {v_key::none, alt_key::shift}, // 0x25
    {v_key::none, alt_key::shift}, // 0x26
    {v_key::none, alt_key::shift}, // 0x27
    {v_key::none, alt_key::shift}, // 0x28
    {v_key::none, alt_key::shift}, // 0x29
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
    {v_key::none, alt_key::shift}, // 0x3D
    {}, // 0x3E
    {}, // 0x3F
    {}, // 0x40
    {v_key::A, alt_key::shift}, // 0x41
    {v_key::B, alt_key::shift}, // 0x42
    {v_key::C, alt_key::shift}, // 0x43
    {v_key::D, alt_key::shift}, // 0x44
    {v_key::E, alt_key::shift}, // 0x45
    {v_key::F, alt_key::shift}, // 0x46
    {v_key::G, alt_key::shift}, // 0x47
    {v_key::H, alt_key::shift}, // 0x48
    {v_key::I, alt_key::shift}, // 0x49
    {v_key::J, alt_key::shift}, // 0x4A
    {v_key::K, alt_key::shift}, // 0x4B
    {v_key::L, alt_key::shift}, // 0x4C
    {v_key::M, alt_key::shift}, // 0x4D
    {v_key::N, alt_key::shift}, // 0x4E
    {v_key::O, alt_key::shift}, // 0x4F
    {v_key::P, alt_key::shift}, // 0x50
    {v_key::Q, alt_key::shift}, // 0x51
    {v_key::R, alt_key::shift}, // 0x52
    {v_key::S, alt_key::shift}, // 0x53
    {v_key::T, alt_key::shift}, // 0x54
    {v_key::U, alt_key::shift}, // 0x55
    {v_key::V, alt_key::shift}, // 0x56
    {v_key::W, alt_key::shift}, // 0x57
    {v_key::X, alt_key::shift}, // 0x58
    {v_key::Y, alt_key::shift}, // 0x59
    {v_key::Z, alt_key::shift}, // 0x5A
    {}, // 0x5B
    {}, // 0x5C
    {}, // 0x5D
    {}, // 0x5E
    {}, // 0x5F
    {}, // 0x60
    {v_key::A, alt_key::none}, // 0x61
    {v_key::B, alt_key::none}, // 0x62
    {v_key::C, alt_key::none}, // 0x63
    {v_key::D, alt_key::none}, // 0x64
    {v_key::E, alt_key::none}, // 0x65
    {v_key::F, alt_key::none}, // 0x66
    {v_key::G, alt_key::none}, // 0x67
    {v_key::H, alt_key::none}, // 0x68
    {v_key::I, alt_key::none}, // 0x69
    {v_key::J, alt_key::none}, // 0x6A
    {v_key::K, alt_key::none}, // 0x6B
    {v_key::L, alt_key::none}, // 0x6C
    {v_key::M, alt_key::none}, // 0x6D
    {v_key::N, alt_key::none}, // 0x6E
    {v_key::O, alt_key::none}, // 0x6F
    {v_key::P, alt_key::none}, // 0x70
    {v_key::Q, alt_key::none}, // 0x71
    {v_key::R, alt_key::none}, // 0x72
    {v_key::S, alt_key::none}, // 0x73
    {v_key::T, alt_key::none}, // 0x74
    {v_key::U, alt_key::none}, // 0x75
    {v_key::V, alt_key::none}, // 0x76
    {v_key::W, alt_key::none}, // 0x77
    {v_key::X, alt_key::none}, // 0x78
    {v_key::Y, alt_key::none}, // 0x79
    {v_key::Z, alt_key::none}, // 0x7A
    {}, // 0x7B
    {}, // 0x7C
    {}, // 0x7D
    {}, // 0x7E
    {v_key::backspace, alt_key::none}, // 0x7F
//    {}, // 0x80
//    {}, // 0x81
//    {}, // 0x82
//    {}, // 0x83
//    {}, // 0x84
//    {}, // 0x85
//    {}, // 0x86
//    {}, // 0x87
//    {}, // 0x88
//    {}, // 0x89
//    {}, // 0x8A
//    {}, // 0x8B
//    {}, // 0x8C
//    {}, // 0x8D
//    {}, // 0x8E
//    {}, // 0x8F
//    {}, // 0x90
//    {}, // 0x91
//    {}, // 0x92
//    {}, // 0x93
//    {}, // 0x94
//    {}, // 0x95
//    {}, // 0x96
//    {}, // 0x97
//    {}, // 0x98
//    {}, // 0x99
//    {}, // 0x9A
//    {}, // 0x9B
//    {}, // 0x9C
//    {}, // 0x9D
//    {}, // 0x9E
//    {}, // 0x9F
//    {}, // 0xA0
//    {}, // 0xA1
//    {}, // 0xA2
//    {}, // 0xA3
//    {}, // 0xA4
//    {}, // 0xA5
//    {}, // 0xA6
//    {}, // 0xA7
//    {}, // 0xA8
//    {}, // 0xA9
//    {}, // 0xAA
//    {}, // 0xAB
//    {}, // 0xAC
//    {}, // 0xAD
//    {}, // 0xAE
//    {}, // 0xAF
//    {}, // 0xB0
//    {}, // 0xB1
//    {}, // 0xB2
//    {}, // 0xB3
//    {}, // 0xB4
//    {}, // 0xB5
//    {}, // 0xB6
//    {}, // 0xB7
//    {}, // 0xB8
//    {}, // 0xB9
//    {}, // 0xBA
//    {}, // 0xBB
//    {}, // 0xBC
//    {}, // 0xBD
//    {}, // 0xBE
//    {}, // 0xBF
//    {}, // 0xC0
//    {}, // 0xC1
//    {}, // 0xC2
//    {}, // 0xC3
//    {}, // 0xC4
//    {}, // 0xC5
//    {}, // 0xC6
//    {}, // 0xC7
//    {}, // 0xC8
//    {}, // 0xC9
//    {}, // 0xCA
//    {}, // 0xCB
//    {}, // 0xCC
//    {}, // 0xCD
//    {}, // 0xCE
//    {}, // 0xCF
//    {}, // 0xD0
//    {}, // 0xD1
//    {}, // 0xD2
//    {}, // 0xD3
//    {}, // 0xD4
//    {}, // 0xD5
//    {}, // 0xD6
//    {}, // 0xD7
//    {}, // 0xD8
//    {}, // 0xD9
//    {}, // 0xDA
//    {}, // 0xDB
//    {}, // 0xDC
//    {}, // 0xDD
//    {}, // 0xDE
//    {}, // 0xDF
//    {}, // 0xE0
//    {}, // 0xE1
//    {}, // 0xE2
//    {}, // 0xE3
//    {}, // 0xE4
//    {}, // 0xE5
//    {}, // 0xE6
//    {}, // 0xE7
//    {}, // 0xE8
//    {}, // 0xE9
//    {}, // 0xEA
//    {}, // 0xEB
//    {}, // 0xEC
//    {}, // 0xED
//    {}, // 0xEE
//    {}, // 0xEF
//    {}, // 0xF0
//    {}, // 0xF1
//    {}, // 0xF2
//    {}, // 0xF3
//    {}, // 0xF4
//    {}, // 0xF5
//    {}, // 0xF6
//    {}, // 0xF7
//    {}, // 0xF8
//    {}, // 0xF9
//    {}, // 0xFA
//    {}, // 0xFB
//    {}, // 0xFC
//    {}, // 0xFD
//    {}, // 0xFE
//    {}, // 0xFF
};

struct key {
    char        char_value;
    v_key command;
    alt_key     alt;
};

void key_to_string (key k) {
    std::cout
        << "'" << k.char_value << "'"
        << " c: " << std::hex << (uint8_t)k.command
        << " a:" << std::hex << (uint8_t)k.alt << std::endl;
}

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

template < std::size_t len >
bool match (uint8_t * buffer, char const (&lit)[len]) {
    return std::equal(buffer, buffer + len, lit);
}

key parse_input () {
    uint8_t buffer [] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int r = 0;

    r = read(STDIN_FILENO, +buffer, sizeof(buffer));

    if (r == 0)
        return {};

    if (r == 1 && buffer[0] < 0x80) {
        return {
            *reinterpret_cast < char *> (+buffer),
            key_map [buffer[0]].control,
            key_map [buffer[0]].alt
        };
    } else {
        // detect alt
        if (r == 2 && buffer[0] == '\x1B') {
            return {
                *reinterpret_cast < char *> (1 + buffer),
                key_map [buffer[1]].control,
                key_map [buffer[1]].alt | alt_key::alt
            };
        } else if (r > 2 && match(buffer, "\x1B[")) {
            auto * k = (+buffer + 2);

/*
            switch (*k) {
                // cursor keys
                case 'x41':
                    return {0, v_key::up, alt_key::none};
                case 'x42':
                    return {0, v_key::down, alt_key::none};
                case 'x43':
                    return {0, v_key::right, alt_key::none};
                case 'x44':
                    return {0, v_key::left, alt_key::none};
            }
            */
            /*
            // detect f keys
            if (match(k, "[A"))
                return {0, v_key::f1, alt_key::none };
            else if (match(k, "[B"))
                return {0, v_key::f2, alt_key::none };
            else if (match(k, "[C"))
                return {0, v_key::f3, alt_key::none };
            else if (match(k, "[D"))
                return {0, v_key::f4, alt_key::none };
            else if (match(k, "[E"))
                return {0, v_key::f5, alt_key::none };
            else if (match(k, "17~"))
                return {0, v_key::f6, alt_key::none };
            else if (match(k, "18~"))
                return {0, v_key::f7, alt_key::none };
            else if (match(k, "19~"))
                return {0, v_key::f8, alt_key::none };
            else if (match(k, "20~"))
                return {0, v_key::f9, alt_key::none };
            else if (match(k, "21~"))
                return {0, v_key::f10, alt_key::none };
            else if (match(k, "23~"))
                return {0, v_key::f11, alt_key::none };
            else if (match(k, "23"))
                return {0, v_key::f12, alt_key::none };
            */
        }

        return {};
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

	new_settings.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	new_settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
	new_settings.c_cflag &= ~(OPOST);
	new_settings.c_cflag |= CS8;
	new_settings.c_cc[VMIN] = 0;
	new_settings.c_cc[VTIME] = 1;

	tcsetattr(0, TCSANOW, &new_settings);
	fcntl(STDIN_FILENO, F_SETFL, FNDELAY);

    //while (i = getchar()){
	while(true) {
	    /*
	    auto v = parse_input();

	    if (v.char_value == 0)
	        continue;

	    key_to_string(v);

	    if (v.alt == alt_key::control && v.char_value == 'c')
	        break;
        */

        uint8_t buffer [] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        int r = 0;

        r = read(STDIN_FILENO, +buffer, 8);

		if (r != EOF) {
		    std::cout << std::dec;
            //std::cout << "r: " << r << " str: " << reinterpret_cast < char * > (+buffer) << std::endl;
            std::cout << "r: " << r << " | ";
            std::cout << std::hex;
            std::cout << "hex: ";
            for (int i = 0; i < 8; ++i)  {
                std::cout << " ";

                std::cout.fill('0');
                std::cout.width(2);
                std::cout << (int)buffer[i];
            }
            std::cout << std::endl;

            if (buffer[0] == 3)
                break;
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

		//if (args.key == (v_key::control | v_key::C))
		//	a.exit();
	};

	app.run();

	/*
    std::cin >> i;

    tui_device.enable_cursor(true);
    tui_device.fullscreen(false);
	*/

    return 0;
}