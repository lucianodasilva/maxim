#ifdef __linux__

#include "maxim.output.unix.driver.hpp"
#include "maxim.stream.hpp"

#include <termio.h>
#include <unistd.h>
#include <iostream>

namespace maxim::output::_unix {

    driver::driver () {}

    size driver::get_size() const {
        winsize w = {};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        return {
            w.ws_col,
            w.ws_row
        };
    }

    void driver::send(span < char > data) const {
        ::write (STDOUT_FILENO, data.data (), data.size ());
    }

}

#endif