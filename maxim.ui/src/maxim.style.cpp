#include "maxim.style.hpp"

#include "maxim.button.hpp"
#include "maxim.listbox.hpp"
#include "maxim.textbox.hpp"

#include <maxim.io.hpp>


namespace maxim {

	style::style (style_desc const & desc) {
		// set all default properties
		this->set_properties (DEFAULT_CLASS_NAME, states::DEFAULT, desc.properties);
		// set all default states
		this->set_states (DEFAULT_CLASS_NAME, desc.states);
		// set all classes
		this->set_classes (desc.classes);
	}

	style & style::set_properties (std::string_view class_name, std::string_view state_name, style_state_desc const & properties) {
		style_property_key key = { class_name, state_name };

		for (auto const & [property_name, value] : properties) {
			key.property = property_name;
			this->set (key, value);
		}

		return *this;
	}

	style & style::set_states(std::string_view class_name, const maxim::style_state_desc_map &states) {
		for (auto const & [state_name, properties] : states) {
			this->set_properties (class_name, state_name, properties);
		}

		return *this;
	}

	style & style::set_classes(const maxim::style_class_desc_map &classes) {
		for (auto const & [class_name, class_instance] : classes) {
			// set all default properties
			this->set_properties (class_name, states::DEFAULT, class_instance.properties);

			// set all state properties
			this->set_states (class_name, class_instance.states);

			// register class inheritance
			_inheritance[class_name] = class_instance.from;
		}

		return *this;
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
		static style const style_inst ({
			.classes = {
				{ listbox::DEFAULT_STYLE_CLASS_NAME, {
					.states = {
						{ states::FOCUSED, {
							{ properties::BACKGROUND, defaults::ACCENT_DARK },
							{ properties::TEXT_COLOR, defaults::ACCENT_DARK.contrast_color() }
						}},
						{ states::SELECTED, {
							{ properties::BACKGROUND, defaults::ACCENT },
							{ properties::TEXT_COLOR, defaults::ACCENT.contrast_color() }
						}},
					},
					.properties = {
						{ properties::SCROLLBAR, true },
						{ properties::SCROLLBAR_COLOR, defaults::SEC_LIGHT },
						{ properties::SCROLLBAR_BACKGROUND, defaults::SEC },
					}
				}},
				{ button::DEFAULT_STYLE_CLASS_NAME, {
					.states = {
						{ states::FOCUSED, {
							{ properties::BACKGROUND, defaults::ACCENT },
							{ properties::TEXT_COLOR, defaults::ACCENT.contrast_color() }
						}},
						{ states::PRESSED, {
	                        { properties::BACKGROUND, defaults::ACCENT_LIGHT },
		                    { properties::TEXT_COLOR, defaults::ACCENT_LIGHT.contrast_color() }
	                    }},
					},
					.properties = {
						{ properties::BACKGROUND, defaults::ACCENT_DARK },
						{ properties::TEXT_COLOR, defaults::ACCENT_DARK.contrast_color() }
					}
				}},
				{ textbox::DEFAULT_STYLE_CLASS_NAME, {
					.states = {
						{ states::FOCUSED, {
							{ properties::BACKGROUND, defaults::ACCENT },
							{ properties::TEXT_COLOR, defaults::ACCENT.contrast_color() }
						}},
					},
					.properties = {
						{ properties::BACKGROUND, defaults::ACCENT_DARK },
						{ properties::TEXT_COLOR, defaults::SEC_DARK.contrast_color() }
					}
				}},
			},
			.states = {
				{ states::FOCUSED, {
					{ properties::BACKGROUND, defaults::ACCENT_DARK },
					{ properties::TEXT_COLOR, defaults::ACCENT_DARK.contrast_color() }
				}},
				{ states::SELECTED, {
					{ properties::BACKGROUND, defaults::ACCENT },
					{ properties::TEXT_COLOR, defaults::ACCENT.contrast_color() }
				}},
			},
			.properties = {
				{ properties::BACKGROUND, defaults::NEUTRAL },
				{ properties::BORDER, false },
				{ properties::TEXT_COLOR, defaults::NEUTRAL.contrast_color() },
				{ properties::TEXT_ATTRIBUTES, defaults::TEXT_ATTRIBUTES },
				}
			});

		return style_inst;
	}

	style_property_reader::style_property_reader (style const & style, style_property_key const & base_key) :
		_style (style),
		_base_key (base_key)
	{}

	void style_client::style_class_name_reset () {
		this->_style_class_name = default_style_class_name();
	}

	std::string_view style_client::style_class_name() {
		if (not _style_class_name.has_value()) {
			this->style_class_name_reset ();
		}

		return _style_class_name.value();
	}

	void style_client::style_class_name(std::string_view class_name) {
		_style_class_name = class_name;
	}

	style_property_reader style_client::style_reader () {
		return {
			this->style(),
			{ this->style_class_name(), this->style_state () },
		};
	}
}