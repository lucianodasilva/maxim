#pragma once
#ifndef MAXIM_PROGRESS_BAR_CPP_HPP
#define MAXIM_PROGRESS_BAR_CPP_HPP

#include "maxim.control.hpp"

namespace maxim {

	class progress_bar : public control {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "progress-bar";
		std::string_view        default_style_class_name() const override;

		void on_paint(maxim::graphics &g, maxim::rect bounds) override;

		property < int >            min = 0;
		property < int >            max = 0;
		property < int >            value = 0;

		property < std::string >    text;
	};

}

#endif
