#pragma once
#ifndef _maxim_layout_hpp_
#define _maxim_layout_hpp_

#include <type_traits>
#include <cinttypes>

namespace maxim {

    enum struct docking_type : uint8_t {
        fill    = 0x0,
        left    = 0x1,
        right   = 0x2,
        top     = 0x4,
        bottom  = 0x8
    };

    inline docking_type operator | (docking_type v1, docking_type v2) {
        using under_type = std::underlying_type_t < docking_type >;

        return static_cast <docking_type> (
                static_cast <under_type> (v1) |
                static_cast <under_type> (v2)
        );
    }

    inline docking_type operator & (docking_type v1, docking_type v2) {
        using under_type = std::underlying_type_t < docking_type >;

        return static_cast <docking_type> (
                static_cast <under_type> (v1) &
                static_cast <under_type> (v2)
        );
    }

}

#endif
