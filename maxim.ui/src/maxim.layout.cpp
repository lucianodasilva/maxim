#include "maxim.layout.hpp"
#include "maxim.details.hpp"

namespace maxim::layout {

	maxim::rect apply_margins (maxim::rect bounds, margins const & margins) {
		return {
			bounds.position + margins.near,
			{
				std::max (bounds.size.width - (margins.near.x + margins.far.x), 0),
				std::max (bounds.size.height - (margins.near.y + margins.far.y), 0)
			}
		};
	}

}