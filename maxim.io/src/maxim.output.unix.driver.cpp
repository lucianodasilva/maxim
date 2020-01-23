#ifdef __linux__

#include "maxim.output.unix.driver.hpp"
#include "maxim.stream.hpp"

#include <termio.h>
#include <unistd.h>

namespace maxim::output::_unix {

    size driver::get_size() const {
        winsize w = {};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        return {
            w.ws_col,
            w.ws_row
        };
    }

    void driver::send(maxim::stream &stream) const {
        ::write(STDOUT_FILENO, stream.data(), stream.size());
    }

    void driver::flush() const {
		tcflush(STDOUT_FILENO, TCOFLUSH);
	}

}

#endif