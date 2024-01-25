#pragma once
#ifndef _maxim_ui_panel_hpp_
#define _maxim_ui_panel_hpp_

#include "maxim.control_container.hpp"
#include "maxim.layout.hpp"

namespace maxim {

	enum struct anchor_kind : uint8_t {
		none        = 0,
		top	        = 1 << 0,
		bottom      = 1 << 1,
		left        = 1 << 2,
		right       = 1 << 3,
	};

	enum struct docking_kind : uint8_t {
		none        = 0,
		top         = 1,
		bottom      = 2,
		left        = 3,
		right       = 4,
		fill        = 5,
	};

	class panel_item : public virtual control {
	public:
		property < maxim::point >           position;
		property < maxim::point >           size;
		property < maxim::anchor_kind >     anchor { maxim::anchor_kind::none };
		property < maxim::docking_kind >    docking { maxim::docking_kind::none };
	};

	class panel : public composite_control_container < panel_item > {
	public:
		constexpr static auto   DEFAULT_STYLE_CLASS_NAME = "panel";
		std::string_view        default_style_class_name () const override;

		void on_paint (maxim::graphics & g, maxim::size size) override;

		property < point >          default_size {{ 100, 100 }};
		property < maxim::margins > margins;

	private:
		maxim::rect item_layout(panel_item * item, maxim::size size);
	};

	template < typename derived_type >
	class builder < panel_item, derived_type > {
	public:
		derived_type & position (maxim::point value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->position = value;

			return inst;
		}

		derived_type & size (maxim::point value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->size = value;

			return inst;
		}

		derived_type & anchor (maxim::anchor_kind value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->anchor = value;

			return inst;
		}

		derived_type & docking (maxim::docking_kind value) {
			auto & inst = *static_cast < derived_type * > (this);
			inst.get()->docking = value;

			return inst;
		}
	};

	template < typename derived_type >
	class builder < panel, derived_type > : public builder < control, derived_type > {
	public:
		using builder < control, derived_type >::builder;

		derived_type & default_size (maxim::point value) {
			this->get()->default_size = value;
			return this->derived();
		}

		derived_type & margins (maxim::margins margins) {
			this->get()->margins = margins;
			return this->derived ();
		}
	};

}

#endif
