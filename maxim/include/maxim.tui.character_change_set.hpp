#pragma once
#ifndef _MAXIM_TUI_CHARACTER_CHANGE_SET_HPP_
#define _MAXIM_TUI_CHARACTER_CHANGE_SET_HPP_

#include <cinttypes>
#include <vector>

namespace maxim {
    namespace tui {

        struct character_span {
            uint32_t begin, end;
        };

        using character_change_set = std::vector < character_span >;

    }
}

#endif //_MAXIM_TUI_CHARACTER_CHANGE_SET_HPP_
