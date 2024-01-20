#include "maxim.layout.hpp"
#include "maxim.details.hpp"

namespace maxim::layout {

	maxim::rect apply_margins (maxim::rect bounds, margins const & margins) {
		bounds = bounds.offset(margins.near).expand(maxim::point () - (margins.near - margins.far));
		// trim negative values
		bounds.width = std::max (bounds.width, 0);
		bounds.height = std::max (bounds.height, 0);

		return bounds;
	}

}