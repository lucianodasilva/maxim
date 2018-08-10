#include "maxim.term.output.driver.hpp"

#ifdef __linux__
#	include "maxim.term.output.unix.driver.hpp"
	std::unique_ptr < maxim::term::output::driver > maxim::term::output::driver::create() {
	return std::make_unique < maxim::term::output::_unix::driver >();
	}
#endif

#ifdef WIN32
#	include "maxim.term.output.win.driver.hpp"
	std::unique_ptr < maxim::term::output::driver > maxim::term::output::driver::create() {
		return std::make_unique < maxim::term::output::win::driver >();
	}
#endif