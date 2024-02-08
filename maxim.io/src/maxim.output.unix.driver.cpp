#ifdef __linux__

#include "maxim.output.unix.driver.hpp"
#include "maxim.stream.hpp"

#include <termio.h>
#include <unistd.h>
#include <sys/stat.h>
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
        flockfile (stdout);

        for (auto const c : data) {
            putchar_unlocked (c);
        }

        funlockfile(stdout);
        fflush (stdout);
    }

}

#endif