#include "maxim.term.input.driver.hpp"

#ifdef __linux__
#   include "maxim.term.input.unix.driver.hpp"
    std::unique_ptr < maxim::term::input::driver > maxim::term::input::driver::create() {
        return std::make_unique < maxim::term::input::_unix::driver > ();
    }
#endif