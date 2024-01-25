#pragma once
#ifndef _maxim_ui_paintable_hpp_
#define _maxim_ui_paintable_hpp_

#include <maxim.io.hpp>
#include "maxim.event.hpp"

namespace maxim {

	class paintable {
	public:
		virtual ~paintable () = default;
		virtual void on_paint (graphics & g, maxim::size size) = 0;
	};

	template < typename derived_type >
	using paint_event = event < derived_type & /* sender */, maxim::graphics & /* g */, maxim::size /* size */>;

	template < typename derived_type >
	class basic_paintable : public paintable {
	public:
		paint_event < derived_type > paint;

		void on_paint (graphics & g, maxim::size size) override {
			paint.invoke (*static_cast < derived_type * > (this), g, size);
		}
	};
}

#endif