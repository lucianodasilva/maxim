#pragma once
#ifndef _maxim_ui_progress_bar_hpp_
#define _maxim_ui_progress_bar_hpp_

#include "maxim.control.hpp"

namespace maxim {

	class progress_bar : public basic_control {
	public:
		using builder = specialized_builder < progress_bar >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "progress-bar";
		std::string_view        default_style_class_name() const override;

		void on_paint(maxim::graphics &g, maxim::rect bounds) override;

		property < int >            min = 0;
		property < int >            max = 0;
		property < int >            value = 0;

		property < std::string >    text;
	};

	template < typename derived_type >
	class builder < progress_bar, derived_type > : public builder < basic_control, derived_type > {
	public:
		using builder < basic_control, derived_type >::builder;

		inline derived_type & min (int value) {
			this->get()->value;
			return this->derived();
		}

		inline derived_type & max (int value) {
			this->get()->max = value;
			return this->derived();
		}

		inline derived_type & value (int value) {
			this->get()->value = value;
			return this->derived();
		}

		inline derived_type & text (std::string value) {
			this->get()->text = value;
			return this->derived();
		}
	};

}

#endif
