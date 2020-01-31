#include "maxim.label.hpp"

namespace maxim {

    void label::paint (graphics & g, rect const & bounds) {
        // render background as default
        element::paint (g, bounds);

        // -----------
        text_style_t text_style;

        text_style.alignment = text_alignment::near;
        text_style.line_alignment = text_alignment::near;
        text_style.wrap = true;

        // render text
        g.draw_text(
			text.get(),
			bounds,
			style.get(),
			text_style
        );

    }

}