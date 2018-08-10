#ifdef __linux__

namespace maxim {
    namespace term {
    	namespace output {

			size driver::get_size() const {
				winsize w = {};
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

				return {
					w.ws_col,
					w.ws_row
				};
			}

			void driver::send(maxim::tui::stream &stream) const {
				::write(STDOUT_FILENO, stream.data(), stream.size());
			}

		}

    }
}

#endif