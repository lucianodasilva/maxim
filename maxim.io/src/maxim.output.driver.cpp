#include "maxim.output.driver.hpp"

#ifdef __linux__
#	include "maxim.output.unix.driver.hpp"
	std::unique_ptr < maxim::output::driver > maxim::output::driver::create() {
	return std::make_unique < maxim::output::_unix::driver >();
	}
#endif

#ifdef WIN32
#	include "maxim.output.win.driver.hpp"
	std::unique_ptr < maxim::output::driver > maxim::output::driver::create() {
		return std::make_unique < maxim::output::win::driver >();
	}
#endif