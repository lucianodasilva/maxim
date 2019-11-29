#ifdef __linux__

#include <thread>

#include <maxim.io.hpp>
#include <maxim.input.unix.key_state_emulator.hpp>
#include <functional>

#include <catch2/catch.hpp>

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

TEST_CASE("single key press", "[unix key state emulator]") {
	using namespace maxim;
	using namespace maxim::input;
	using namespace maxim::input::_unix;

	using namespace std;

	key_state_emulator emulator;

    class : public maxim::input_event_handler {
    public:

        void on_key_up (maxim::input::key_info const & key) override {
            ++key_up_count;
            last_key = key;
        }

        void on_key_down (maxim::input::key_info const & key) override {
            ++key_down_count;
            last_key = key;
        }

        std::size_t key_down_count{ 0 };
        std::size_t key_up_count{ 0 };
        key_info	last_key{};

    } test_state;

	key_info
		key{
			'D',
			virtual_key::D,
			modifier_key::shift
	},
		another_key{
			'F',
			virtual_key::F,
			modifier_key::none
	},
		null_key{};

	SECTION("single key up event") {
		emulator.evaluate_and_dispatch(key, test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 0);
		REQUIRE(test_state.last_key == key);

		wait_for_expiration();
		emulator.timeout_and_dispatch(test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 1);
		REQUIRE(test_state.last_key == key);
	}

	SECTION("single key up event no wait") {

		emulator.evaluate_and_dispatch(key, test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 0);
		REQUIRE(test_state.last_key == key);

		emulator.timeout_and_dispatch(test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 0);
		REQUIRE(test_state.last_key == key);
	}

	SECTION("duplicate key press half wait") {

		emulator.evaluate_and_dispatch (key, test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 0);
		REQUIRE(test_state.last_key == key);

		emulator.timeout_and_dispatch(test_state);

		REQUIRE(test_state.key_down_count == 1);
		REQUIRE(test_state.key_up_count == 0);
		REQUIRE(test_state.last_key == key);

        wait_half_expiration();

        emulator.evaluate_and_dispatch (key, test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 0);
        REQUIRE(test_state.last_key == key);

        emulator.timeout_and_dispatch(test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 0);
        REQUIRE(test_state.last_key == key);

        wait_for_expiration();

        emulator.timeout_and_dispatch(test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 1);
        REQUIRE(test_state.last_key == key);
	}

    SECTION("duplicate key full wait") {

        emulator.evaluate_and_dispatch (key, test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 0);
        REQUIRE(test_state.last_key == key);

        emulator.timeout_and_dispatch(test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 0);
        REQUIRE(test_state.last_key == key);

        wait_for_expiration();
        emulator.timeout_and_dispatch(test_state);

        REQUIRE(test_state.key_down_count == 1);
        REQUIRE(test_state.key_up_count == 1);
        REQUIRE(test_state.last_key == key);

        emulator.evaluate_and_dispatch (key, test_state);

        REQUIRE(test_state.key_down_count == 2);
        REQUIRE(test_state.key_up_count == 1);
        REQUIRE(test_state.last_key == key);

        wait_for_expiration();
        emulator.timeout_and_dispatch(test_state);

        REQUIRE(test_state.key_down_count == 2);
        REQUIRE(test_state.key_up_count == 2);
        REQUIRE(test_state.last_key == key);
    }

}

#endif