#ifdef __linux__

#include <catch2/catch.hpp>

#include <maxim.io.hpp>
#include <maxim.input.unix.termio_parser.hpp>

TEST_CASE("basic key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("single character parsing") {
        char const buffer [] = "a";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::A);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("single character with single modifier parsing") {
        char const buffer [] = "\003";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::C);
        REQUIRE(k.modifiers == modifier_key::control);
    }

    SECTION("single character with multiple modifier parsing") {
        char const buffer [] = "\033J";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::J);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }

    SECTION("single escape character") {
        char const buffer [1] = {'\033'};
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::escape);
        REQUIRE(k.modifiers == modifier_key::none);
    }
}

TEST_CASE("function key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("low f_key simple parsing sse3") {
        char const buffer [] = "\033OP";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f1);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("low f_key with single modifier parsing") {
        char const buffer [] = "\033[12;2~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f2);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("low f_key with no modifier parsing") {
        char const buffer [] = "\033[14~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f4);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("low f_key with multiple modifier parsing") {
        char const buffer [] = "\033[13;4~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f3);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }

    SECTION("high f_key simple parsing") {
        char const buffer [] = "\033[20~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f9);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("high f_key with single modifier parsing") {
        char const buffer [] = "\033[17;2~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f6);
        REQUIRE(k.modifiers == modifier_key::shift);
    }


    SECTION("high f_key with multiple modifier parsing") {
        char const buffer [] = "\033[18;7~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::f7);
        REQUIRE((k.modifiers & modifier_key::control) == modifier_key::control);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("special key presses", "[unix termio parser]") {

    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'insert' simple parsing") {
        char const buffer [] = "\033[2";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse (source);

        REQUIRE(k.key == virtual_key::insert);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'delete' key with single modifier parsing") {
        char const buffer [] = "\033[3;2~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse (source);

        REQUIRE(k.key == virtual_key::del);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'home' with multiple modifier parsing") {
        char const buffer [] = "\033[H;8~";
        span source { +buffer, +buffer + sizeof (buffer) };
    
        auto k = termio_parse(source);
    
        REQUIRE(k.key == virtual_key::home);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::control) == modifier_key::control);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}

TEST_CASE("directional key presses", "[unix termio parser]") {

    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'up' simple parsing") {
        char const buffer [] = "\033[A~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::up);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'down' key with single modifier parsing") {
        char const buffer [] = "\033[B;2~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::down);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'right' with multiple modifier parsing") {
        char const buffer [] = "\033[C;6~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::right);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::control) == modifier_key::control);
    }

}

TEST_CASE("page up/down key presses", "[unix termio parser]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    SECTION("'page up' simple parsing") {
        char const buffer [] = "\033[5~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::page_up);
        REQUIRE(k.modifiers == modifier_key::none);
    }

    SECTION("'page up' key with single modifier parsing") {
        char const buffer [] = "\033[5;2~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::page_up);
        REQUIRE(k.modifiers == modifier_key::shift);
    }

    SECTION("'page down' with multiple modifier parsing") {
        char const buffer [] = "\033[6;4~";
        span source { +buffer, +buffer + sizeof (buffer) };

        auto k = termio_parse(source);

        REQUIRE(k.key == virtual_key::page_down);
        REQUIRE((k.modifiers & modifier_key::shift) == modifier_key::shift);
        REQUIRE((k.modifiers & modifier_key::alt) == modifier_key::alt);
    }
}


#endif