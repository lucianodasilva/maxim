#pragma once
#ifndef _maxim_paintable_hpp_
#define _maxim_paintable_hpp_

#include <maxim.io.hpp>
#include "maxim.event.hpp"

namespace maxim {

	class paintable_base {
	public:
		virtual ~paintable_base () = default;
		virtual void on_paint (graphics & g, rect bounds) = 0;
	};

	template < typename sender_type >
	using paint_event = event < sender_type &, graphics &, rect const & >;

	template < typename sender_type >
	class paintable : public paintable_base {
	public:

		paint_event < sender_type > paint;

		void on_paint (graphics & g, rect bounds) override {
			paint.invoke (*static_cast < sender_type * > (this), g, bounds);
		}
	};

}

#endif