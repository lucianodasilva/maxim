#pragma once
#ifndef MAXIM_UI_NUMUPDOWN_HPP
#define MAXIM_UI_NUMUPDOWN_HPP

#include "maxim.control.hpp"
#include "maxim.label.hpp"

namespace maxim {

	class numupdown : public virtual maxim::control, private maxim::label {
	public:
		using builder = specialized_builder < numupdown >;

		numupdown ();

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "numupdown";
		std::string_view        default_style_class_name() const override;

		property < int >        value = 0;
		property < int >        min = 0;
		property < int >        max = 0;
		property < int >        step = 1;

		void                    on_paint (maxim::graphics & g, maxim::rect bounds) override;
		void                    on_key_press (maxim::input::key_info const & e) override;

	};

	template < typename derived_type >
	class builder < numupdown, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		inline derived_type & value (int value) {
			this->get()->value = value;
			return this->derived();
		}

		inline derived_type & min (int value) {
			this->get()->min = value;
			return this->derived();
		}

		inline derived_type & max (int value) {
			this->get()->max = value;
			return this->derived();
		}

		inline derived_type & step (int value) {
			this->get()->step = value;
			return this->derived();
		}
	};

}

#endif
