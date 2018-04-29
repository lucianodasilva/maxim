#pragma once
#ifndef _MAXIM_TUI_DRIVER_HPP_
#define _MAXIM_TUI_DRIVER_HPP_

#include "maxim.tui.structures.hpp"

namespace maxim {
    namespace tui {

		struct stream;

        class driver {
        public:

			driver();

			size get_size() const;

			void send(maxim::tui::stream & stream);

        };

    }
}

#endif //_MAXIM_TUI_DRIVER_HPP_
