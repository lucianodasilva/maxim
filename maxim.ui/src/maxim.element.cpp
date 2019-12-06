#include "maxim.element.hpp"

namespace maxim {

    void element::paint (graphics & g, rect const & bounds) {
        g.draw_box(bounds, background.get(), true, background.get());
    }

}