
append_source (. maxim-demo-source-files)

add_executable(maxim-demo ${maxim-demo-source-files})

target_link_libraries (
        maxim-demo
        PUBLIC
            maxim.io)