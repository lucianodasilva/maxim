#include <catch.hpp>
#include <maxim.device.hpp>

TEST_CASE("calculate change set", "[character_buffer calculate_change_set]") {
    using namespace maxim;

    character_buffer const empty_buffer {{ 5, 5}};

    SECTION("validate empty buffers") {
        character_buffer const compare_buffer { empty_buffer.size() };

        auto result = compare_buffer.calculate_change_set(empty_buffer);

        REQUIRE (result.empty());
    }

    SECTION("single character vs empty") {

        character_buffer compare_buffer { empty_buffer.size() };

        point const change_position { 3, 3};
        auto const change_index = compare_buffer.coords_to_index(change_position);

        auto & c = *compare_buffer.ptr_at (change_position);

        c.attributes.forecolor = colors::cyan;

        auto result = compare_buffer.calculate_change_set(empty_buffer);

        REQUIRE (result.size() == 1);
        REQUIRE (result[0].begin == change_index);
        REQUIRE (result[0].end == change_index + 1);

    }

    SECTION("span vs empty") {

        character_buffer compare_buffer { empty_buffer.size() };

        point const change_position_1 { 3, 3};
        point const change_position_2 { 4, 3};
        auto const change_index = compare_buffer.coords_to_index(change_position_1);

        auto & c1 = *compare_buffer.ptr_at (change_position_1);
        auto & c2 = *compare_buffer.ptr_at (change_position_2);

        c1.attributes.forecolor = colors::cyan;
        c2.attributes.forecolor = colors::cyan;

        auto result = compare_buffer.calculate_change_set(empty_buffer);

        REQUIRE (result.size() == 1);
        REQUIRE (result[0].begin == change_index);
        REQUIRE (result[0].end == change_index + 2);

    }

}