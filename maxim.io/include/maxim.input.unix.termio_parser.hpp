#pragma once
#ifndef MAXIM_INPUT_UNIX_TERMIO_PARSER_HPP
#define MAXIM_INPUT_UNIX_TERMIO_PARSER_HPP

#include "maxim.input.hpp"

namespace maxim::input::unix {

    key_info termio_parse(std::string_view source);

}

#endif
