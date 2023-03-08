append_source (. maxim-io-test-files)

find_package (Catch2 CONFIG REQUIRED)

add_executable (maxim-io-test ${maxim-io-test-files})
target_link_libraries(maxim-io-test PUBLIC maxim.io Catch2::Catch2 Catch2::Catch2WithMain)