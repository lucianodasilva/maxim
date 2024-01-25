#pragma once
#ifndef _maxim_ui_grid_hpp_
#define _maxim_ui_grid_hpp_

#include "maxim.control_container.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	class grid_item : public virtual control {
	public:
		property < layout::partition_range > columns;
		property < layout::partition_range > rows;

		static maxim::rect layout (class grid * parent, grid_item * item, maxim::rect bounds);
	};

	class grid : public composite_control_container < grid_item > {
	public:
		using builder = specialized_builder < grid >;

		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "grid";
		std::string_view        default_style_class_name () const override;

		std::vector < layout::partition > columns { 1.0_var };
		std::vector < layout::partition > rows { 1.0_var };

		property < maxim::margins > margins;
		property < maxim::point >   padding;

		void on_initialize_controls () override;
		void on_paint (graphics & g, maxim::size size) override;
	};

	template < typename derived_type >
	class builder < grid, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		auto & columns (std::vector < layout::partition > value) {
			this->get()->columns = value;
			return this->derived();
		}

		auto & rows (std::vector < layout::partition > value) {
			this->get()->rows = value;
			return this->derived();
		}

		auto & layout (std::vector < layout::partition > columns, std::vector < layout::partition > rows) {
			this->get()->columns = columns;
			this->get()->rows = rows;

			return this->derived();
		}

		auto & margins (maxim::margins value) {
			this->get()->margins = value;
			return this->derived();
		}

		auto & padding (maxim::point value) {
			this->get()->padding = value;
			return this->derived();
		}

	};

	template < typename derived_type >
	class builder < grid_item, derived_type > {
	public:
		derived_type & position (layout::partition_range column_range, layout::partition_range row_range) {
			auto & inst = *static_cast < derived_type * > (this);

			inst.get()->columns = column_range;
			inst.get()->rows = row_range;

			return inst;
		}
	};
}

#endif
