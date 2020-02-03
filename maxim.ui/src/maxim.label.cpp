#include "maxim.label.hpp"

namespace maxim {

    void label::paint (graphics & g, rect const & bounds) {
        // render background as default
        element::paint (g, bounds);

        // render text
        g.draw_text(
			text.get(),
			bounds,
			style.get(),
			text_style.get()
        );

    }

}