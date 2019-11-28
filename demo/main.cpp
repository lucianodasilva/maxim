#include <maxim.hpp>
#include <iostream>
#include <algorithm>
#include <array>

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

int main(int arg_c, char ** arg_v)
{
	using namespace maxim;

	application app;

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

	//app.on_draw += &render;

    //app.device().fullscreen(true);
    //app.device().enable_cursor(false);
    //app.device().clear_screen();
    //app.device().send_thing("\033]10;?\007");

	app.run();

    return 0;
}