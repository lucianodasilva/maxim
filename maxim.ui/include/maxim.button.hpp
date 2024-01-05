#pragma once
#ifndef MAXIM_BUTTON_HPP
#define MAXIM_BUTTON_HPP

#include "maxim.label.hpp"

namespace maxim {

	using pressed_event = event < class button & /* sender */ >;

	class button : public label {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "button";
		std::string_view default_style_class_name () const override;

		std::string_view style_state () const override;

		pressed_event pressed;
		virtual void on_pressed ();

		void on_key_down (maxim::input::key_info const & key) override;
		void on_key_up (maxim::input::key_info const & key) override;

		inline bool is_pressed () const { return _pressed; }

	private:
		bool _pressed = false;
	};
}

#endif
