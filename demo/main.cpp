#include <maxim.io.hpp>
#include <maxim.ui.hpp>
#include <iostream>
#include <algorithm>
#include <array>
#include <sstream>
#include <maxim.encoding.hpp>

/*
struct menu_item {
public:

    std::string const           text;
    std::function < void () >   callback;
};

struct menu {
public:

    std::vector < menu_item > items;
    int selected_index { 0 };

    void render (maxim::graphics & g) {
        using namespace maxim;

        auto width = get_max_width () + 4;
        auto position = point { g.view.size.width / 2 - width / 2, 4 };

        for (int i = 0; i < items.size(); ++i) {

            auto def = get_default_char (i);
            g.draw_text(items[i].text, position + point {0, i * 2}, width, alignment::centered, def.forecolor, def.backcolor, def.flags);
        }

    }

    void select_down () {
        ++selected_index;

        if (selected_index >= items.size())
            selected_index = 0;
    }

    void select_up () {
        --selected_index;

        if (selected_index < 0)
            selected_index = items.size() - 1;
    }

    void do_select () {
        if (items[selected_index].callback)
            items  [selected_index].callback();
    }

private:

    int get_max_width () {
        int w = 0;

        for (auto const & item : items)
            w = std::max (static_cast < int > (item.text.size()), w);

        return w;
    }

    maxim::character get_default_char (int index) {
        using namespace maxim;

        if (index == selected_index) {
            return {
                ' ',
                {},
                colors::green,
                colors::dark_gray
            };
        } else {
            return {
                ' ',
                {},
                colors::dark_gray,
                colors::white
            };
        }
    }

};

menu * active_menu_page { nullptr };

void render (maxim::application & app, maxim::graphics & g) {

    g.clear();

    auto size = g.view.size;

    g.draw_h_line({0, 0}, size.width - 1, maxim::colors::dark_green);

    if (active_menu_page)
        active_menu_page->render (g);

    g.draw_h_line({0, size.height - 1}, size.width - 1, maxim::colors::dark_green);

}
*/

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;

	application app;

	/*
	menu m;

	m.items.push_back({"Battery Level"});
    m.items.push_back({"NV Memory Test"});
    m.items.push_back({"Reset NV"});
    m.items.push_back({"Self-Destruct"});
    m.items.push_back({"Exit", [&]() { app.exit(); }});

    active_menu_page = &m;

	app.on_key_down += [](application & a, input::key_info const & args) {

	    if (active_menu_page) {
            if (args.key == input::virtual_key::down) {
                active_menu_page->select_down();
            }

            if (args.key == input::virtual_key::up) {
                active_menu_page->select_up();
            }

            if (args.key == input::virtual_key::enter) {
                active_menu_page->do_select();
            }
        }

		if (args.key == input::virtual_key::C && args.modifiers == input::modifier_key::control) {
			a.exit();
		}

	};

	app.on_draw += &render;
	 */

    app.on_key_down += [](application & a, input::key_info const & args) {
        if (args.key == input::virtual_key::C && args.modifiers == input::modifier_key::control) {
            a.exit();
         }
    };

    app.on_draw += [](application & a, maxim::graphics & g) {
        std::stringstream s;
        s << "size: " << g.view.size.width << ", " << g.view.size.height;

        constexpr style_t style {
        	maxim::colors::red,
        	maxim::colors::white
        };

        g.draw_text (s.str(), maxim::point { 1, 1 }, style);
        //g.draw_v_line (maxim::point { 20, 2 }, 40, colors::cyan);

        /*
        auto & stream = a.device().stream();

        stream
        	// clear
        	// .write (encoding::ansi::clear)
        	// position
        	.write (encoding::ansi::color_reset)
			.write (encoding::ansi::move_prefix)
			.write (2)
			.write (";")
			.write (2)
			.write (encoding::ansi::move_suffix)
        	// color
        	.write (encoding::ansi::color_prefix)
        	.write (encoding::ansi::color_foreground)
        	.write ((int)colors::green)
        	.write (encoding::ansi::color_suffix)
        	// text
        	.write ("cenas")
			.write (encoding::ansi::color_prefix)
			.write (encoding::ansi::color_background)
			.write ((int)colors::gray)
			.write (encoding::ansi::color_suffix)
			.write (" coiso ")
			// --------------
			.write (encoding::ansi::color_prefix)
			//.write (encoding::ansi::color_background)
			.write ("39;49")
			.write (encoding::ansi::color_suffix)
			.write (" yada ");*/
    };

    app.device().set_fullscreen(true);
    app.device().set_cursor_enabled(false);
    app.device().clear_screen();

    // make controls

    /*
    grid main_panel;

    main_panel.columns = { 10.0_var, 10.0_var, 10.0_var };
    main_panel.rows = { 5.0_var, 5.0_var, 5.0_var };

    auto * name_label_1 = main_panel.create_element < label > ();

    name_label_1->text = "1";
    name_label_1->text_alignment = maxim::alignment::centered;
    name_label_1->columns = grid::range { 0 };
    name_label_1->rows = grid::range {0 };
    name_label_1->background = maxim::colors::yellow;
    name_label_1->foreground = maxim::colors::red;

    auto * name_label_2 = main_panel.create_element < label > ();

    name_label_2->text = "2";
    name_label_2->text_alignment = maxim::alignment::centered;
    name_label_2->columns = grid::range { 1 };
    name_label_2->rows = grid::range {0};
    name_label_2->background = maxim::colors::red;
    name_label_2->foreground = maxim::colors::green;

    auto * name_label_3 = main_panel.create_element < label > ();

    name_label_3->text = "3";
    name_label_3->text_alignment = maxim::alignment::centered;
    name_label_3->columns = grid::range { 2 };
    name_label_3->rows = grid::range {0};
    name_label_3->background = maxim::colors::green;
    name_label_3->foreground = maxim::colors::blue;

    auto * name_label_4 = main_panel.create_element < label > ();

    name_label_4->text = "4";
    name_label_4->text_alignment = maxim::alignment::centered;
    name_label_4->columns = grid::range { 0 };
    name_label_4->rows = grid::range {1};
    name_label_4->background = maxim::colors::blue;
    name_label_4->foreground = maxim::colors::cyan;

    auto * name_label_5  = main_panel.create_element < label > ();

    name_label_5->text = "5";
    name_label_5->text_alignment = maxim::alignment::centered;
    name_label_5->columns = grid::range { 1 };
    name_label_5->rows = grid::range {1};
    name_label_5->background = maxim::colors::cyan;
    name_label_5->foreground = maxim::colors::white;

    auto * name_label_6 = main_panel.create_element < label > ();

    name_label_6->text = "6";
    name_label_6->text_alignment = maxim::alignment::centered;
    name_label_6->columns = grid::range { 2 };
    name_label_6->rows = grid::range {1};
    name_label_6->background = maxim::colors::white;
    name_label_6->foreground = maxim::colors::black;

    auto * name_label_7 = main_panel.create_element < label > ();

    name_label_7->text = "7";
    name_label_7->text_alignment = maxim::alignment::centered;
    name_label_7->columns = grid::range { 0 };
    name_label_7->rows = grid::range {2};
    name_label_7->background = maxim::colors::black;
    name_label_7->foreground = maxim::colors::pink;

    auto * name_label_8 = main_panel.create_element < label > ();

    name_label_8->text = "8";
    name_label_8->text_alignment = maxim::alignment::centered;
    name_label_8->columns = grid::range { 1 };
    name_label_8->rows = grid::range {2};
    name_label_8->background = maxim::colors::pink;
    name_label_8->foreground = maxim::colors::purple;

    auto * name_label_9 = main_panel.create_element < label > ();

    name_label_9->text = "9";
    name_label_9->text_alignment = maxim::alignment::centered;
    name_label_9->columns = grid::range { 2 };
    name_label_9->rows = grid::range {2 };
    name_label_9->background = maxim::colors::purple;
    name_label_9->foreground = maxim::colors::yellow;

    app.root = &main_panel;
    */

	app.run();

    return 0;
}