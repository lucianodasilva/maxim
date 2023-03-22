
append_source (./include maxim-io-include-files)
append_source (./src maxim-io-source-files)

# create target
add_library (maxim.io
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
        $<$<CXX_COMPILER_ID:GNU,CLANG>:Threads::Threads>)

target_compile_features (
        maxim.io PUBLIC cxx_std_17
)

if (MAXIM_BUILD_TEST)
    add_subdirectory(test)
endif()