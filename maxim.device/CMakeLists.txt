
dir_target_name (target_name)

target_compile_features(
    ${target_name}
    PUBLIC
    cxx_std_17)

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

        find_program(lcov_path lcov)
        find_program(genhtml_path genhtml)

        if (NOT lcov_path OR NOT genhtml_path)
            message (FATAL_ERROR "lcov or genhtml not found!")
        endif()

        # TODO: move to external script file
        add_custom_target(${target_name}_coverage
                WORKING_DIRECTORY $<TARGET_FILE_DIR:${target_name}_test>
                # clean stuffs
                COMMAND echo \"Cleaning coverage counters\"
                COMMAND ${lcov_path} --directory . --zerocounters --quiet --rc lcov_branch_coverage=1
                # get counters from compilation
                COMMAND echo \"Gathering pre-execution coverage counters\"
                COMMAND ${lcov_path} --directory . --initial --capture --output-file initial.cov --quiet
                # execute tests to generate runtime counters
                COMMAND echo \"Execute tests...\"
                COMMAND $<TARGET_FILE:${target_name}_test>
                # filter results
                COMMAND echo \"Gathering pos-execution coverage counters\"
                COMMAND ${lcov_path} --directory . --capture --output-file exec.cov --quiet --rc lcov_branch_coverage=1
                COMMAND ${lcov_path} --add-tracefile initial.cov --add-tracefile exec.cov --output-file full.cov --quiet --rc lcov_branch_coverage=1

                COMMAND echo \"Filtering...\"
                COMMAND ${lcov_path} --extract full.cov '${CMAKE_CURRENT_LIST_DIR}/*' --output-file filtered.cov --quiet --rc lcov_branch_coverage=1
                COMMAND ${lcov_path} --remove filtered.cov '${CMAKE_CURRENT_LIST_DIR}/test/*' --output-file filtered.cov --quiet --rc lcov_branch_coverage=1
                # generate html report
                COMMAND echo \"Generating html report\"
                COMMAND ${genhtml_path} filtered.cov --output-directory ./cov_report --demangle-cpp --rc lcov_branch_coverage=1 --quiet
                COMMAND echo \"Open 'file://$<TARGET_FILE_DIR:${target_name}_test>/cov_report/index.html' for the coverage report\"
                COMMAND ${lcov_path} --summary filtered.cov --rc lcov_branch_coverage=1)

    endif()
endif()