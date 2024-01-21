#include "maxim.tab_panel.hpp"
#include "maxim.application.hpp"

namespace maxim {

	std::string_view tab_panel_button::style_state () const {
		if (!host_tab_panel) {
			return states::DEFAULT;
		}

		bool is_hovered = this->has_focus();
		bool is_selected = host_tab_panel->selected_index () == this->tab_index;

		return is_selected ?
		       (is_hovered ? states::SELECTED_FOCUSED : states::SELECTED) :
               (is_hovered ? states::FOCUSED : states::DEFAULT);
	}

	std::string_view tab_panel::default_style_class_name () const {
		return DEFAULT_STYLE_CLASS_NAME;
	}

	std::string_view tab_panel::style_state () const {
		return states::SELECTED;
	}

	void tab_panel::on_initialize_controls() {
		basic_control::on_initialize_controls();

		_tab_button_stack.orientation = orientation::horizontal;

		// share tab focus index ( reserve 0 for tab buttons )
		_tab_button_stack.focus_index (this->focus_index ().append(0));
		// share tree position
		_tab_button_stack.attach_to_tree(this->application(), this->parent());
		// set same style class
		_tab_button_stack.style_class_name.bind (this->style_class_name);

		_tab_button_stack.on_initialize_controls();

		this->bind_style_property(margins, properties::MARGINS);
	}

	void tab_panel::on_activate() {
		basic_control::on_activate();

		_tab_button_stack.accept([](control * control) {
			control->on_activate();
			return true;
		});
	}

	void tab_panel::on_deactivate() {
		basic_control::on_deactivate();

		_tab_button_stack.accept ([this](control * control) {
			control->on_deactivate();
			return true;
		});
	}

	void tab_panel::on_paint (maxim::graphics & g, maxim::rect bounds) {
		// render itself
		basic_control::on_paint (g, bounds);

		// render the tab buttons
		_tab_button_stack.on_paint (g, { bounds.x, bounds.y, bounds.width, 1 });

		// if there is a selected tab
		if (this->has_selected()) {
			auto selected_bounds = maxim::rect { bounds.x, bounds.y + 1, bounds.width, bounds.height - 1 };
			selected_bounds = layout::apply_margins(selected_bounds, margins);

			// render the selected tab
			auto * selected_tab = tabs ()[_selected_index].get ();
			selected_tab->on_paint (g, selected_bounds);
		}
	}

	void tab_panel::select (int index) {
		if (index < 0 || index >= tabs ().size ()) {
			return;
		}

		if (this->has_selected()) {
			// remove from the focus set
			tabs()[_selected_index]->on_deactivate();
		}

		_selected_index = index;

		// add to the focus set
		tabs()[_selected_index]->on_activate();
		// TODO: think aboud this one. on_key_press is called twice
		// -- tabs()[_selected_index]->focus();
	}

	void tab_panel::select (tab_panel_item * item) {
		if (!item) {
			return;
		}

		auto it = std::find_if (tabs ().begin (), tabs ().end (), [item](auto & tab) {
			return tab.get () == item;
		});

		if (it != tabs ().end ()) {
			select (std::distance (tabs ().begin (), it));
		}
	}

	int tab_panel::selected_index () const {
		return _selected_index;
	}

	tab_panel_item * tab_panel::selected () const {
		if (_selected_index < 0 || _selected_index >= tabs ().size ()) {
			return nullptr;
		}

		return tabs ()[_selected_index].get ();
	}

	void tab_panel::accept (control_visitor_t visitor) const {
		for (auto & control : tabs ()) {
			if (visitor (control.get ())) {
				return;
			}
		}
	}

	std::vector < std::unique_ptr < tab_panel_item > > const & tab_panel::tabs () const {
		return _tabs;
	}

}