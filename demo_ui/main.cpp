#include <maxim.io.hpp>
#include <maxim.ui.hpp>

namespace demo {

    constexpr auto SEGMENT_CLASS_NAME = "segment";

    class stack_demo : public maxim::stack {
    public:

        void on_initialize_controls() override {
            using namespace maxim;

            stack::on_initialize_controls();

            this->orientation = maxim::orientation::vertical;

            // first segment
            this->build_control < label >()
                .style_class_name(SEGMENT_CLASS_NAME)
                .length(1)
                .text("Static Stack Segment");

            auto* static_segment = this->build_control < stack >()
                .horizontal()
                .length(3)
                .get();

            static_segment->build_control < button >().text("Static Button 1").length(17);
            static_segment->build_control < button >().text("Static Button 2").length(17);
            static_segment->build_control < button >().text("Static Button 3").length(17);
            
            // second segment
            this->build_control < label > ()
                .style_class_name(SEGMENT_CLASS_NAME)
                .length(1)
	            .text_attributes({ maxim::alignment::center, maxim::alignment::center })
                .text("Dynamic Stack Segment With Padding (Center Aligned Text)");

            auto* dynamic_segment = this->build_control < stack >()
                .horizontal()
				.length(1_var)
				.padding (1)
				.get();

            dynamic_segment->build_control < button >().text("Button 1").length(1_var);
			dynamic_segment->build_control < button >().text("Button 2").length(2_var);
			dynamic_segment->build_control < button >().text("Button 3").length(3_var);

			// third segment
			this->build_control < label > ()
				.style_class_name(SEGMENT_CLASS_NAME)
				.length(1)
				.margins ({ 0, 0, 1, 0})
				.text_attributes ({ maxim::alignment::far, maxim::alignment::center })
				.text("(Right Aligned Text) Padded Stack Elements With Padding And Margins");

			auto* padded_segment = this->build_control < stack >()
				.horizontal()
				.length(5)
				.padding (1)
				.margins ({1, 1, 1, 1})
				.get();

            padded_segment->build_control < button >().text("Button 1").length(1_var);
            padded_segment->build_control < button >().text("Button 2").length(1_var);
            padded_segment->build_control < button >().text("Button 3").length(1_var);
        }

    };

    class panel_demo : public maxim::panel {
    public:

        void on_initialize_controls() override {
            using namespace maxim;
            panel::on_initialize_controls();

            this->default_size = { 100, 100 };

            // simple anchored list box ==================================================
            this->build_control < label >()
                .text("Listbox (Anchored Up/Down)")
                .position({ 2, 1 })
                .size({ 30, 1 });

            auto* list = this->build_control < listbox >()
                .position({ 2, 2 })
                .size({ 30, 96 })
                .anchor(enum_cat(maxim::anchor_kind::top, maxim::anchor_kind::bottom))
                .get();

            for (int i = 1; i <= 30; i++) {
                list->items.get().push_back({ "Item " + std::to_string(i) });
            }

            // variable rendering list box =================================================
            this->build_control < label >()
                .text("Listbox (Anchored All Around)")
                .position({ 34, 1 })
                .anchor (enum_cat (maxim::anchor_kind::left, maxim::anchor_kind::right))
                .size({ 30, 1 });

            // variable rendering list box renderer
            auto* list2 = this->build_control < listbox >()
                .position({ 34, 2 })
                .size({ 30, 96 })
                .anchor(enum_cat(maxim::anchor_kind::left, maxim::anchor_kind::right, maxim::anchor_kind::top, maxim::anchor_kind::bottom))
                .get();

            list2->item_renderer->wrap_text = true;

            list2->items.get().push_back({ "Lorem ipsum dolor sit amet" });
            list2->items.get().push_back({ "consectetur adipiscing elit" });
            list2->items.get().push_back({ "In eget leo eget dolor cursus condimentum a id urna" });
            list2->items.get().push_back({ "Aenean sed interdum purus" });
            list2->items.get().push_back({ "vitae malesuada nibh" });
            list2->items.get().push_back({ "Ut porttitor lacus eget massa pellentesque lobortis quis ut mi" });
            list2->items.get().push_back({ "Donec bibendum mattis feugiat" });
            list2->items.get().push_back({ "Suspendisse tincidunt egestas nunc" });
            list2->items.get().push_back({ "Nulla cursus enim augue" });
            list2->items.get().push_back({ "sed venenatis lorem auctor vel" });
            list2->items.get().push_back({ "Donec et dapibus quam" });
            list2->items.get().push_back({ "eget auctor est" });
            list2->items.get().push_back({ "Proin feugiat orci vitae odio ultrices" });
            list2->items.get().push_back({ "Nulla imperdiet venenatis nunc, sed cursus tellus bibendum lobortis" });
            list2->items.get().push_back({ "Curabitur bibendum diam quis est sollicitudin auctor" });
            list2->items.get().push_back({ "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas" });
            list2->items.get().push_back({ "Nunc erat risus" });
            list2->items.get().push_back({ "sodales sagittis ipsum et" });
            list2->items.get().push_back({ "aliquam cursus tortor" });
 
            // docked label ==================================================
            this->build_control < label >()
                .position({ 0, 0 })
                .size({ 100, 1 })
                .docking(maxim::docking_kind::bottom)
                .text ("Docked Label (Bottom)");
        }

    };

    class grid_demo : public maxim::grid {
    public:

        void on_initialize_controls() override {
            using namespace maxim;
            grid::on_initialize_controls();

			this->columns = { 1_var, 1_var, 1_var };
			this->rows = { 1_var, 1_var, 1_var };

			this->padding = { 1, 1 };
			this->margins = {1, 1, 1, 1};

			this->build_control < button > ()
			    .position(0, 0)
				.text ("On Cell (0,0)");

			this->build_control < button > ()
			    .position({1, 2}, 0)
				.text ("On Cells [1,0] to [2, 0]");

			this->build_control < button > ()
			    .position(2, {1, 2})
				.text ("On Cells [2, 1] to [2, 2]");
        }
    };

    class tab_demo_panel : public maxim::tab_panel {
    public:

        void on_initialize_controls() override {
			using namespace maxim;

			tab_panel::on_initialize_controls();

            this->build_tab < stack_demo >()
                .tab_caption("Stack Demo")
                .tab_length(12);

            this->build_tab < panel_demo >()
                .tab_caption("Panel Demo")
				.tab_length(12);

            this->build_tab < grid_demo >()
				.tab_caption("Grid Demo")
                .tab_length(12);
		}

	};;

    void configure_app(maxim::application& app) {
        using namespace maxim;

        app.device().set_fullscreen(true);
        app.device().set_cursor_enabled(false);

        app.key_down.append_no_scope([](auto& app, maxim::input::key_info const& e) {
            if (e.matches(maxim::input::virtual_key::escape) || e.matches(maxim::input::virtual_key::Q)) {
                app.stop();
            }
		});

        app.style = maxim::style::default_style();

        // minor tweak to the default style
        app.style.get()
            .set_class(SEGMENT_CLASS_NAME, {
                { states::DEFAULT, {
                    { properties::BACKGROUND, defaults::SEC},
					{ properties::TEXT_COLOR, defaults::SEC.contrast_color()},
                    { properties::TEXT_ATTRIBUTES, maxim::text_attributes {
                        maxim::alignment::near,     // horizontal
                        maxim::alignment::center,    // vertical
                    }},
                    { properties::MARGINS, maxim::margins { 1, 0, 0, 0 }},
                }}});

        app.push_active < demo::tab_demo_panel >();
	}
}

int main(int arg_c, char ** arg_v)
{
    // initialize the application
    maxim::application app;

    demo::configure_app(app);
    app.run();

    return 0;
}