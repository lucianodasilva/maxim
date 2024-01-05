#pragma once
#ifndef MAXIM_CHECKBOX_HPP
#define MAXIM_CHECKBOX_HPP

#include "maxim.label.hpp"

namespace maxim {

	class checkbox : public label {
	public:

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "checkbox";
		std::string_view        default_style_class_name() const override;

		void on_paint(maxim::graphics & g, maxim::rect bounds) override;

		// botton related properties
		property < bool >                   value;
		property < std::string >            text;

		void on_key_press (maxim::input::key_info const & key) override;
	};
}

#endif
