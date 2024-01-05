#pragma once
#ifndef _maxim_grid_hpp_
#define _maxim_grid_hpp_

#include "maxim.control.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	class basic_grid_item {
	public:
		property < layout::partition_range > columns;
		property < layout::partition_range > rows;

		maxim::rect layout (class grid * parent, maxim::rect bounds) const;
	};

	template < typename control_type,  typename std::enable_if_t < std::is_base_of_v < control, control_type >, int > = 0 >
	class grid_item : public basic_grid_item, public virtual control_type {
	public:
		using control_type::control_type;

		inline void on_paint (graphics & g, rect bounds) override {
			// HACK: dynamic_cast is a major code smell... but its what i got for now
			control_type::on_paint (g, this->layout (dynamic_cast < grid * > (this->parent()), bounds));
		}
	};

	class grid : public specialized_control_container < basic_grid_item, grid_item > {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "grid";
		std::string_view        default_style_class_name () const override;

		std::vector < layout::partition > columns { 1.0_var };
		std::vector < layout::partition > rows { 1.0_var };

		void on_paint (graphics & g, rect bounds) override;
	};

}

#endif
