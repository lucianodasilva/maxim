#ifdef __linux__

#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

#include "maxim.tui.driver.hpp"
#include "maxim.tui.stream.hpp"

namespace maxim {
    namespace tui {

        driver::driver() {}

        size driver::get_size() const {
            winsize w = {};
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

            return {
                    w.ws_col,
                    w.ws_row
            };
        }

        void driver::send (maxim::tui::stream & stream) {
            ::write(STDOUT_FILENO, stream.data(), stream.size());
        }

    }
}

#endif