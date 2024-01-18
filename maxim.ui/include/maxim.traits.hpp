#pragma once
#ifndef _maxim_ui_traits_hpp_
#define _maxim_ui_traits_hpp_

#include <functional>
#include <type_traits>

namespace maxim {

	template < typename num_t >
	constexpr auto deferred_forward (std::remove_reference_t < num_t > & value,
	                                 std::enable_if_t < std::is_reference_v < num_t >, int > = 0) noexcept
	{
		return std::ref (value);
	}

	template < typename num_t >
	constexpr auto deferred_forward (std::remove_reference_t < num_t > & value,
	                                 std::enable_if_t < !std::is_reference_v < num_t >, int > = 0) noexcept
	{
		return std::forward <num_t > (value);
	}

	template < typename num_t >
	constexpr auto deferred_forward (std::remove_reference_t < num_t > && value) noexcept {
		return std::forward <num_t > (value);
	}

}

#endif
