#pragma once
#ifndef _maxim_ui_style_hpp_
#define _maxim_ui_style_hpp_

#include <any>
#include <stdexcept>
#include <string_view>
#include <string>
#include <map>
#include <optional>

#include <maxim.io.hpp>
#include "maxim.property.hpp"
#include "maxim.style_property.hpp"

namespace maxim {

	/// Property definitions that compose a style state
	using style_state_desc = std::map < std::string_view, std::any >;

	/// Map of style states
	using style_state_desc_map = std::map < std::string_view, style_state_desc >;

	/// Map of style class links
	using style_inheritance_map = std::unordered_map < std::string_view, std::string_view >;

	struct style {
	public:

		style () = default;

		/// add property(ies) to a class/state pair
		/// @param class_name The class name
		/// @param state_name The state name
		/// @param properties The properties to add
		style & set_state (std::string_view class_name, std::string_view state_name, style_state_desc const & properties);

		/// add state(s) to a class
		/// @param class_name The class name
		/// @param states The states to add
		style & set_class (std::string_view class_name, style_state_desc_map const & states);

		/// add state(s) to a class
		/// @param class_name The class name
		/// @param inherits The class to inherit from
		/// @param states The states to add
		style& set_class(std::string_view class_name, std::string_view inherits, style_state_desc_map const& states);


		/// set a property
		/// @param key The property key
		/// @param value The property value
		template < typename value_type >
		inline style & set (style_property_key key, value_type const & value) {
			_properties.emplace (key, value);
			return *this;
		}

		/// get a property
		/// @param key The property key
		/// @return The property value
		template < typename value_type >
		inline std::optional < value_type > get (style_property_key key) const {
			auto opt_any = this->get_any (key);

			if (!opt_any.has_value()) {
				return std::nullopt;
			}

			if (opt_any->type () != typeid (value_type)) {
				return std::nullopt;
			}

			return std::any_cast < value_type > (opt_any.value());
		}

		/// get a property
		/// @param key The property key
		/// @param value The property value
		/// @param default_value The default value if the property is not found
		template < typename value_type >
		inline style const & get (style_property_key key, value_type & value, value_type default_value = {}) const {
			auto opt_value = get < value_type > (key);

			if (opt_value.has_value ()) {
				value = opt_value.value ();
			} else {
				value = default_value;
			}

			return *this;
		}

		/// Get the default style
		static style const & default_style ();

	private:
		std::optional < std::any > get_any (style_property_key const & key) const;

		style_property_map     _properties;
		style_inheritance_map  _inheritance;
	};

	/// Interface for objects that can be styled
	class style_client {
	public:
		virtual ~style_client () = default;
		virtual maxim::style &          style ()= 0;
		virtual maxim::style const &    style () const = 0;
		virtual std::string_view 	    style_state () const = 0;

		virtual std::string_view        default_style_class_name () const = 0;

		property < std::string_view >   style_class_name;
		void                            style_class_name_reset ();
	};

	/// Helper common property names
	namespace properties {
		constexpr auto BACKGROUND = "background";
		constexpr auto BORDER = "border";
		constexpr auto BORDER_COLOR = "border-color";
		constexpr auto TEXT_COLOR = "text-color";
		constexpr auto TEXT_ATTRIBUTES = "text-attributes";
		constexpr auto SCROLLBAR_BACKGROUND = "scrollbar-background";
		constexpr auto SCROLLBAR_COLOR = "scrollbar-color";
		constexpr auto SCROLLBAR = "scrollbar";
		constexpr auto PADDING = "padding";
		constexpr auto MARGINS = "margins";
		constexpr auto CONTENT_MARGINS = "content-margins";
	};

	/// Helper common state names
	namespace states {
		constexpr auto DEFAULT = "";
		constexpr auto FOCUSED = "focused";
		constexpr auto PRESSED = "pressed";
		constexpr auto SELECTED = "selected";
		constexpr auto SELECTED_FOCUSED = "selected-focused";
	};

	/// Helper common property values
	namespace defaults {
		constexpr auto ACCENT = maxim::color { 0x6337FF };
		constexpr auto ACCENT_LIGHT = maxim::color { 0xAD85FF };
		constexpr auto ACCENT_DARK = maxim::color { 0x150568 };
		constexpr auto SEC = maxim::color { 0x273293 };
		constexpr auto SEC_LIGHT = maxim::color { 0x9891EB };
		constexpr auto SEC_DARK = maxim::color { 0x141746 };
		constexpr auto NEUTRAL = maxim::colors::black;
		constexpr auto SUCCESS = maxim::color { 0x6acc32 };
		constexpr auto SUCCESS_DARK = maxim::color { 0x2f6112 };
		constexpr auto SUCCESS_LIGHT = maxim::color { 0x6acc32 };
		constexpr auto CAUTION = maxim::color { 0xf4b62d };
		constexpr auto CAUTION_LIGHT = maxim::color { 0xc3901e };
		constexpr auto DANGER = maxim::color { 0xeb003d };
		constexpr auto DANGER_DARK = maxim::color { 0xac002a };
		constexpr auto INFO = maxim::color { 0x124fed};
		constexpr auto INFO_DARK = maxim::color { 0x0c174c };

		// no border by default
		constexpr auto BORDER = false;
		constexpr auto BORDER_COLOR = maxim::colors::black;

		// default text attributes
        constexpr auto TEXT_ATTRIBUTES = maxim::text_attributes {
            maxim::alignment::center, // horizontal
            maxim::alignment::center  // vertical
        };
	};

	constexpr auto DEFAULT_CLASS_NAME = "";
}

#endif
