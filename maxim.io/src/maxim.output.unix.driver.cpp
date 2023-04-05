#ifdef __linux__

#include "maxim.output.unix.driver.hpp"
#include "maxim.stream.hpp"

#include <termio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

namespace maxim::output::unix {

    driver::driver () {
        struct stat stats {};
        if (fstat (STDOUT_FILENO, &stats) == -1) {
            throw std::runtime_error ("FStat Failure");
        }

        setvbuf(stdout, nullptr, _IONBF, 0);
    }

    size driver::terminal_size() const {
        winsize w = {};
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        return {
            w.ws_col,
            w.ws_row
        };
    }

    void driver::send(view < char > data) const {
        // TODO: figure this one out better
        //std::cout << std::string (data.data (), data.size()) << '\0' << std::flush;

         for (auto const c : data) {
             putchar(c);
         }

//        auto rem = data.size ();
//
//        do {
//            // tcflush(STDOUT_FILENO, TCOFLUSH);
//            // fflush(stdout);
//            auto ret_code = fwrite(data.begin () + (data.size () - rem), 1, rem, stdout);
//            //auto ret_code = ::write(STDOUT_FILENO, data.begin() + (data.size () - rem), rem);
//
//            if (ret_code > 0) {
//                rem -= ret_code;
//            } else {
//                // error;
//                break;
//            }
//        } while (rem > 0);
        }

}

#endif