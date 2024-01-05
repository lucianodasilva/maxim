#pragma once
#ifndef _maxim_label_hpp_
#define _maxim_label_hpp_

#include "maxim.control.hpp"

namespace maxim {

	class label : public control {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "label";
		std::string_view        default_style_class_name () const override;

		property < std::string > text;

		void on_paint (maxim::graphics & g, maxim::rect bounds) override;
	};

}

#endif
