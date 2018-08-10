#pragma once
#ifndef _maxim_pragma_details_hpp_
#define _maxim_pragma_details_hpp_

#include <type_traits>

namespace maxim {
	
	namespace details {

		// enum tools
		template < 
			typename enum_t, 
			typename = std::enable_if_t < std::is_enum_v < enum_t > >
		>
		inline bool enum_cat ()


	}

}

#endif