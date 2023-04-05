
# get dependencies
FetchContent_Declare(
    las
    GIT_REPOSITORY https://github.com/lucianodasilva/las.git
    GIT_TAG        68b87578)
    
FetchContent_MakeAvailable(las)

append_source(./include maxim-io-include-files)
append_source(./src maxim-io-source-files)

# create target
add_library(maxim.io
        ${maxim-io-include-files}
        ${maxim-io-source-files})

target_include_directories(maxim.io PUBLIC
        ./include/)

# setup compile options for threading on gnu and clang
set(THREADS_PREFER_PTHREAD_FLAG ON) # Without this flag CMake may resort to just '-lpthread'
find_package(Threads)

# add libraries
target_link_libraries (
        maxim.io PUBLIC
        las
        $<$<CXX_COMPILER_ID:GNU,CLANG>:Threads::Threads>)

target_compile_features (
        maxim.io PUBLIC cxx_std_17
)

if (MAXIM_BUILD_TEST)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.3.2)
    
    FetchContent_MakeAvailable(Catch2)

    append_source (. maxim-io-test-files)

    find_package (Catch2 CONFIG REQUIRED)

    add_executable (maxim-io-test ${maxim-io-test-files})
    target_link_libraries(maxim-io-test PUBLIC maxim.io Catch2::Catch2 Catch2::Catch2WithMain)
endif()