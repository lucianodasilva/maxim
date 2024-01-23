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