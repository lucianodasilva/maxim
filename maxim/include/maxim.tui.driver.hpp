#pragma once
#ifndef _MAXIM_TUI_DRIVER_HPP_
#define _MAXIM_TUI_DRIVER_HPP_

#include "maxim.tui.point.hpp"

namespace maxim {
    namespace tui {

		struct stream;

        class driver {
        public:

			driver();

			point get_size() const;

			void send(maxim::tui::stream & stream);

        };

    }
}

#endif //_MAXIM_TUI_DRIVER_HPP_
