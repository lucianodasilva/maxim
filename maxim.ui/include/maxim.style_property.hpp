#pragma once
#ifndef MAXIM_STYLE_PROPERTY_HPP
#define MAXIM_STYLE_PROPERTY_HPP

#include <any>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace maxim {

	/// Key used to identify a style property
	struct style_property_key {
	public:
		std::string_view class_name; 	///< The class name of the property
		std::string_view state;     	///< The state of the property
		std::string_view property;      ///< The property name
	};

	/// Hash function for style_property_key
	struct style_property_hash {
	public:
		std::size_t operator () (style_property_key const & key) const noexcept;
	};

	inline bool operator == (style_property_key const & lhs, style_property_key const & rhs) noexcept {
		return lhs.class_name == rhs.class_name && lhs.state == rhs.state && lhs.property == rhs.property;
	}

	inline bool operator != (style_property_key const & lhs, style_property_key const & rhs) noexcept {
		return !(lhs == rhs);
	}

	/// Cascades a style property key up the inheritance chain
	/// @param key The key to cascade
	/// @param super_class The super class of the key
	/// @param target_state The target state of the key
	/// @return The cascaded key
	/// @note The cascading algorithm is as follows:
	/// 1st - a key with a state will be "stepped up" the inheritance chain to the default state
	/// 2nd - a key with no state will be "stepped up" the inheritance chain to the super class and target state
	/// 3rd - a key with no state and no super class will return nulopt since the root of the chain has been reached
	[[nodiscard]] std::optional < style_property_key > style_property_cascade (
		style_property_key key,
		std::string_view super_class,
		std::string_view target_state) noexcept;

	using style_property_map = std::unordered_map < style_property_key, std::any, style_property_hash >;
}

#endif
