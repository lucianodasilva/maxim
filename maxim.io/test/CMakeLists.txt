dir_target_name (target_name)

target_compile_features(
        ${target_name}
        PUBLIC
        cxx_std_17)

target_link_libraries(${target_name} PUBLIC Catch2::Catch2)

if (MAXIM_BUILD_TEST_COVERAGE)
    if (UNIX)
        # enable code coverage flags
        target_compile_options(
                ${target_name} PUBLIC
                -fprofile-arcs
                -ftest-coverage
                --coverage)

        target_link_libraries(
                ${target_name} PUBLIC
                gcov
        )

    endif()
endif()