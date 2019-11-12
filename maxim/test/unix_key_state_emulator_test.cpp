#ifdef __linux__

#include <catch2/catch.hpp>

#include <maxim.hpp>
#include <maxim.input.unix.key_state_emulator.hpp>

void wait_for_expiration () {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    using namespace std;

    // wait for key expiration
    std::this_thread::sleep_for(
        std::chrono::milliseconds (
            key_state_emulator::key_delay_ms +
            key_state_emulator::key_delay_ms
        )
    );
}

void wait_half_expiration () {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    using namespace std;

    // wait for key expiration
    std::this_thread::sleep_for(
        std::chrono::milliseconds (
            key_state_emulator::key_delay_ms / 2
        )
    );
}

template < class _container_t >
std::ptrdiff_t index_of_key (_container_t const & c, maxim::input::key_event_args const & k) {
    return std::distance(std::begin(c), std::find_if(std::begin(c), std::end(c), [&](maxim::input::key_event_args const & i) {
        return i.key.char_value == k.key.char_value &&
            i.key.key == k.key.key &&
            i.key.modifiers == k.key.modifiers;
    }));
}

TEST_CASE("single key press", "[unix key state emulator]") {
    using namespace maxim::input;
    using namespace maxim::input::_unix;

    using namespace std;

    key_state_emulator emulator;

    key_event_args
        key {
            'D',
            virtual_key::D,
            modifier_key::shift
        },
        another_key {
            'F',
            virtual_key::F,
            modifier_key::none
        },
        null_key {};

    SECTION("single key up event") {

        auto events = emulator.update_key_state(key);

        REQUIRE(events.size () == 1);
        REQUIRE(events[0].key == key.key);
        REQUIRE(events[0].is_down);

        wait_for_expiration();

        events = emulator.update_key_state(null_key);

        REQUIRE(events.size () == 1);
        REQUIRE(events[0].key == key.key);
        REQUIRE(!events[0].is_down);
    }

    SECTION("single key up event no wait") {

        auto events = emulator.update_key_state(key);

        REQUIRE(events.size () == 1);

        auto & ev_item = events[0];

        REQUIRE(ev_item.key == key.key);
        REQUIRE(ev_item.is_down);

        events = emulator.update_key_state(null_key);

        REQUIRE(events.empty());
    }

    SECTION("duplicate key press") {

        auto events = emulator.update_key_state(key);

        REQUIRE(events.size () == 1);

        auto & ev_item = events[0];
        REQUIRE(ev_item.key == key.key);
        REQUIRE(ev_item.is_down);

        wait_half_expiration();

        events = emulator.update_key_state(null_key);

        REQUIRE(events.empty());
    }

    SECTION("key down event and key up detection") {

        auto events = emulator.update_key_state(key);

        REQUIRE(events.size () == 1);

        auto & ev_item = events [0];
        REQUIRE(ev_item.key == key.key);
        REQUIRE(ev_item.is_down);

        wait_for_expiration();

        events = emulator.update_key_state(another_key);

        std::ptrdiff_t
            key_index = index_of_key (events, key),
            another_key_index = index_of_key(events, another_key);

        REQUIRE(key_index >= 0);
        REQUIRE(!events[key_index].is_down);

        REQUIRE(another_key_index >= 0);
        REQUIRE(events[another_key_index].is_down);
    }

}

#endif