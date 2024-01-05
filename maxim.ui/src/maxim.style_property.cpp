#include "maxim.style_property.hpp"
#include "maxim.style.hpp"
#include "maxim.details.hpp"

namespace maxim {

	std::size_t style_property_hash::operator () (style_property_key const & key) const noexcept {
		std::size_t seed = 0;

		maxim::hash::combine (seed, key.class_name, key.state, key.property);
		return seed;
	}

	std::optional < style_property_key > style_property_cascade (
		style_property_key key,
		std::string_view super_class,
		std::string_view target_state) noexcept
	{
		// validate "end of the line"
		if (key.class_name == DEFAULT_CLASS_NAME && key.state == states::DEFAULT) {
			return std::nullopt;
		}

		// copy original key
		auto cascaded_key = key;

		// if we have no state, then we need to "step up" the inheritance chain
		if (cascaded_key.state == states::DEFAULT) {
			cascaded_key.class_name = super_class;
			cascaded_key.state = target_state;
		} else {
			// if we have a state, then we need to "step up" the inheritance chain
			// to the default state
			cascaded_key.state = states::DEFAULT;
		}

		return cascaded_key;
	}

}