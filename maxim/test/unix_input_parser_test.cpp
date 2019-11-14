#ifdef __linux__

#include <catch2/catch.hpp>

#include <maxim.hpp>
#include <maxim.input.unix.termio_parser.hpp>

TEST_CASE("basic key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("single character parsing") {

        uint64_t const
                    input = 0x61;

        auto k = parse_termio_input(input, 1).key;

        REQUIRE(k.key == virtual_key::A);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("single character with single modifier parsing") {

        uint64_t const
            input = 0x03;

        auto k = parse_termio_input(input, 1).key;

        REQUIRE(k.key == virtual_key::C);
        REQUIRE(k.modifiers == modifier_key::control);
    }

    SECTION("single character with multiple modifier parsing") {

        uint64_t const
            input = 0x4A1B;

        auto k = parse_termio_input(input, 2).key;

        REQUIRE(k.key == virtual_key::J);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("function key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("low f_key simple parsing") {

        uint64_t const
            input = 0x504F1B;

        auto k = parse_termio_input(input, 3).key;

        REQUIRE(k.key == virtual_key::f1);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("low f_key with single modifier parsing") {

        uint64_t const
            input = 0x51323B315B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::f2);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("low f_key with multiple modifier parsing") {

        uint64_t const
            input = 0x52343B315B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::f3);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }

    SECTION("high f_key simple parsing") {

        uint64_t const
            input = 0x7E35315B1B;

        auto k = parse_termio_input(input, 5).key;

        REQUIRE(k.key == virtual_key::f5);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("high f_key with single modifier parsing") {

        uint64_t const
            input = 0x7E323B37315B1B;

        auto k = parse_termio_input(input, 7).key;

        REQUIRE(k.key == virtual_key::f6);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("high f_key with multiple modifier parsing") {

        uint64_t const
            input = 0x7E343B38315B1B;

        auto k = parse_termio_input(input, 7).key;

        REQUIRE(k.key == virtual_key::f7);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("special key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'insert' simple parsing") {

        uint64_t const
            input = 0X7E325B1B;

        auto k = parse_termio_input(input, 4).key;

        REQUIRE(k.key == virtual_key::insert);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'delete' key with single modifier parsing") {

        uint64_t const
            input = 0x7E323B335B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::del);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'home' with multiple modifier parsing") {

        uint64_t const
            input = 0x48343B315B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::home);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("directional key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'up' simple parsing") {

        uint64_t const
            input = 0X415B1B;

        auto k = parse_termio_input(input, 3).key;

        REQUIRE(k.key == virtual_key::up);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'down' key with single modifier parsing") {

        uint64_t const
            input = 0x42323B315B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::down);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'right' with multiple modifier parsing") {

        uint64_t const
            input = 0x43343B315B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::right);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("page up/down key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'page up' simple parsing") {

        uint64_t const
            input = 0X7E355B1B;

        auto k = parse_termio_input(input, 4).key;

        REQUIRE(k.key == virtual_key::page_up);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'page up' key with single modifier parsing") {

        uint64_t const
            input = 0x7E323B355B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::page_up);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'page down' with multiple modifier parsing") {

        uint64_t const
            input = 0x7E343B365B1B;

        auto k = parse_termio_input(input, 6).key;

        REQUIRE(k.key == virtual_key::page_down);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

#endif