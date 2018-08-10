#ifdef __linux__

#include "maxim.term.output.unix.driver.hpp"
#include "maxim.term.stream.hpp"

#include <termio.h>
#include <unistd.h>

namespace maxim {
    namespace term {
    	namespace output {
			namespace _unix {

				size driver::get_size() const {
					winsize w = {};
					ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

					return {
						w.ws_col,
						w.ws_row
					};
				}

				void driver::send(maxim::term::stream &stream) const {
					::write(STDOUT_FILENO, stream.data(), stream.size());
				}

			}
		}

    }
}

#endif