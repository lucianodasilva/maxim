#include "maxim.style.hpp"

#include "maxim.button.hpp"
#include "maxim.grid.hpp"
#include "maxim.listbox.hpp"
#include "maxim.stack.hpp"
#include "maxim.textbox.hpp"
#include "maxim.panel.hpp"

#include <maxim.io.hpp>


namespace maxim {

	style & style::set_state (std::string_view class_name, std::string_view state_name, style_state_desc const & properties) {
		style_property_key key = { class_name, state_name };

		for (auto const & [property_name, value] : properties) {
			key.property = property_name;
			this->set (key, value);
		}

		return *this;
	}

	style & style::set_class(std::string_view class_name, const maxim::style_state_desc_map &states) {
		for (auto const & [state_name, properties] : states) {
			this->set_state (class_name, state_name, properties);
		}

		return *this;
	}

	style& style::set_class(std::string_view class_name, std::string_view inherits, const maxim::style_state_desc_map& states) {
		_inheritance [class_name] = inherits;
		return this->set_class (class_name, states);
	}

	std::optional < std::any > style::get_any (style_property_key const & key) const {
		std::optional < style_property_key > opt_key = key;
		auto target_state = key.state;

		do {
			// check for property
			auto it = _properties.find (opt_key.value());

			if (it != _properties.end ()) {
				return it->second;
			}

			// if we have no property, cascade the key up the inheritance chain
			// - check our super class ( TODO: the super class in only needed one every two iterations, perhaps we can optimize )
			auto super_class_it = _inheritance.find (opt_key->class_name);
			auto super_class = std::string_view { super_class_it != _inheritance.end () ? super_class_it->second : DEFAULT_CLASS_NAME };

			// - cascade the key
			opt_key = style_property_cascade (opt_key.value(), super_class, target_state);
		} while (opt_key.has_value ());

		return std::nullopt;
	}

	style const & style::default_style () {
		static auto inst = maxim::style()
			.set_class(DEFAULT_CLASS_NAME, {
				{ states::DEFAULT, {
					{ properties::BACKGROUND, defaults::SEC_DARK },
					{ properties::BORDER, false },
					{ properties::TEXT_COLOR, defaults::SEC_DARK.contrast_color() },
					{ properties::TEXT_ATTRIBUTES, defaults::TEXT_ATTRIBUTES },
				}},
				{ states::FOCUSED, {
					{ properties::BACKGROUND, defaults::ACCENT },
					{ properties::TEXT_COLOR, defaults::ACCENT.contrast_color() }
				}},
				{ states::SELECTED, {
					{ properties::BACKGROUND, defaults::ACCENT_LIGHT },
					{ properties::TEXT_COLOR, defaults::ACCENT_LIGHT.contrast_color() }
				}},
				{ states::SELECTED_FOCUSED, {
					{ properties::BACKGROUND, defaults::ACCENT_LIGHT },
					{ properties::TEXT_COLOR, defaults::ACCENT }
				}},
				{ states::PRESSED, {
					{ properties::BACKGROUND, defaults::ACCENT_LIGHT },
					{ properties::TEXT_COLOR, defaults::ACCENT_LIGHT.contrast_color() }
				}},
				})
			.set({stack::DEFAULT_STYLE_CLASS_NAME, states::DEFAULT, properties::BACKGROUND }, defaults::NEUTRAL)
			.set({grid::DEFAULT_STYLE_CLASS_NAME, states::DEFAULT, properties::BACKGROUND }, defaults::NEUTRAL)
			.set({panel::DEFAULT_STYLE_CLASS_NAME, states::DEFAULT, properties::BACKGROUND }, defaults::NEUTRAL)
			.set({ listbox::DEFAULT_STYLE_CLASS_NAME, states::DEFAULT, properties::BACKGROUND }, defaults::NEUTRAL)
			;

		return inst;
	}

	void style_client::style_class_name_reset () {
		this->style_class_name = this->default_style_class_name();
	}
}