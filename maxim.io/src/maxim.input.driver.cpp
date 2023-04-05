#include "maxim.input.driver.hpp"

#ifdef __linux__
#   include "maxim.input.unix.driver.hpp"
    std::unique_ptr < maxim::input::driver > maxim::input::driver::create() {
        return std::make_unique < maxim::input::unix::driver > ();
    }
#endif

#ifdef WIN32
#	include "maxim.input.win.driver.hpp"
	std::unique_ptr < maxim::input::sec_driver > maxim::input::sec_driver::create() {
		return std::make_unique < maxim::input::win::sec_driver >();
	}
#endif