#pragma once
#ifndef _MAXIM_TUI_DRIVER_HPP_
#define _MAXIM_TUI_DRIVER_HPP_

#include "maxim.tui.structures.hpp"
#include "maxim.tui.input.hpp"

#include <memory>
#include <vector>

namespace maxim {
    namespace tui {

		struct stream;

        class driver {
        public:

			driver();
			virtual ~driver();

			size get_size() const;

			void send(maxim::tui::stream & stream) const;

			std::vector<input_event_info> get_input_events() const;

		private:

			struct internals;
			std::unique_ptr < internals > _internals;

        };

    }
}

#endif //_MAXIM_TUI_DRIVER_HPP_
