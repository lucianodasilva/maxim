#pragma once
#ifndef _maxim_ui_listbox_hpp_
#define _maxim_ui_listbox_hpp_

#include "maxim.control.hpp"

namespace maxim {

	struct listbox_item {
	public:
		std::string text;
		std::any    tag;
	};

	using listbox_selection_changed_event = event < class listbox & /* sender */, std::optional < listbox_item > /* item */ >;

	class listbox :
		public basic_control {
	public:
		using builder = specialized_builder < listbox >;

		listbox ();
		static constexpr int NO_SELECTION = std::numeric_limits < int >::max();

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "listbox";
		std::string_view        default_style_class_name() const override;

		std::string_view        style_state () const override;

		void on_focus () override;

		void on_paint(maxim::graphics &g, maxim::rect bounds) override;

		// functionality related properties
		property<int>  selected_index = NO_SELECTION;

		// capture keyboard events
		void on_key_press (maxim::input::key_info const & key) override;

		listbox_selection_changed_event selection_changed;
		virtual void on_selection_changed (std::optional < listbox_item > item);

		// TODO: perhaps hide this in the implementation?
		property < std::vector < listbox_item > > items;

	protected:
		void update_view (int height);
	private:
		int _last_height = 0;
		int _hovered_offset = 0;
		int _scroll_offset = 0;
	};

	template < typename derived_type >
	class builder < listbox, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;
	};
}

#endif