#pragma once
#ifndef _maxim_term_driver_hpp_
#define _maxim_term_driver_hpp_

#include "maxim.term.structures.hpp"
#include "maxim.term.input.hpp"

#include <memory>
#include <vector>

namespace maxim {
    namespace term {

		struct stream;

        class driver {
        public:

			driver();
			virtual ~driver();

			size get_size() const;

			void send(maxim::term::stream & stream) const;

		private:

			struct internals;
			std::unique_ptr < internals > _internals;

        };

    }
}

#endif
