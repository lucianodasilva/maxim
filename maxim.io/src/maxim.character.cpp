#include "maxim.character.hpp"
#include <stdexcept>

namespace maxim {

    uint8_t color::to_256 () const {
        constexpr int cv_lookup [6] = {0, 0x5F, 0x87, 0xAF, 0xD7, 0xFF};

        // calculate nearest index
        auto channel_to_index = [](auto ... c) {
            return std::make_tuple (c > 115 ? ((c - 35) / 40) : (c > 48 ? 1 : 0)...);
        };

        auto [ir, ig, ib] = channel_to_index (r, g, b);

        auto avr = (ir + ig + ib / 3);
        auto gray_index = (avr > 238 ? 23 : (avr - 3) / 10);

        auto
            cr = cv_lookup[ir],
            cg = cv_lookup[ig],
            cb = cv_lookup[ib];

        auto gv = 8 + 10 * gray_index;

        auto sqr = [](auto v1, auto v2) { return (v1 - v2) * (v1 - v2); };

        auto color_dist = sqr (cr, r) + sqr (cg, g) + sqr (cb, b);
        auto gray_dist = sqr (gv, r) + sqr (gv, g) + sqr (gv, b);

        auto clr = color_dist <= gray_dist ?
                   16 + (36 * ir + 6 * ig + ib) :
                   232 + gray_index;

        return clr;
    }

    uint8_t color::to_8 () const {
        auto value = to_256();

        if (value < 8) {
            return 30 + value;
        }

        if (value < 16) {
            return 90 + (value - 8);
        }

        uint8_t tr, tg, tb;

        if (value >= 232) {
            tr = tg = tb = (((value - 232) * 10) + 8) / 255;
        } else {
            value -= 16;
            auto const rem = value % 16;

            tr = value / 36 / 5;
            tg = rem / 6 / 5;
            tb = (rem % 6) / 5;
        }

        auto const max_chan = std::max ({tr, tg, tb}) * 2;

        if (max_chan == 0) {
            return 30;
        }

        value = 30 + (tb << 2) | (tg << 1) | tr;

        if (max_chan == 2) {
            value += 60;
        }

        return value;
    }
}