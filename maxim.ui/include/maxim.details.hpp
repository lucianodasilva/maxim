#pragma once
#ifndef _maxim_ui_details_hpp_
#define _maxim_ui_details_hpp_

#include <cinttypes>
#include <functional>

namespace maxim {

	namespace hash {
		inline std::uint64_t mix (std::uint64_t value) {
			std::uint64_t constexpr m = 0xe9846af9b1a615d;

			value ^= value >> 32;
			value *= m;
			value ^= value >> 32;
			value *= m;
			value ^= value >> 28;

			return value;
		}

		inline std::uint32_t mix (uint32_t value) {
			std::uint32_t constexpr m1 = 0x21f0aaad;
			std::uint32_t constexpr m2 = 0x735a2d97;

			value ^= value >> 16;
			value *= m1;
			value ^= value >> 15;
			value *= m2;
			value ^= value >> 15;

			return value;
		}

		template < typename value_type >
		inline void combine (std::size_t & seed, value_type const & value) {
			seed = mix (seed + 0x9e3779b9 + std::hash < value_type >()(value));
		}

		template < typename ... args_t >
		inline void combine (std::size_t & seed, args_t const & ... args) {
			(combine (seed, args), ...);
		}
	}

}

#endif
