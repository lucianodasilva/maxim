#pragma once
#ifndef _maxim_ui_button_hpp_
#define _maxim_ui_button_hpp_

#include "maxim.label.hpp"

namespace maxim {

	using pressed_event = event < class button & /* sender */ >;

	class button : public label {
	public:
		using builder = specialized_builder < button >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "button";
		std::string_view default_style_class_name () const override;

		button ();

		std::string_view style_state () const override;

		pressed_event pressed;
		virtual void on_pressed ();

		void on_key_down (maxim::input::key_info const & key) override;
		void on_key_up (maxim::input::key_info const & key) override;

		void on_focus_lost () override;

		inline bool is_pressed () const { return _pressed; }
	private:
		bool _pressed = false;
	};

	template < typename derived_type >
	class builder < button, derived_type > : public builder < label, derived_type > {
	public:
		using builder < label, derived_type >::builder;
	};
}

#endif
