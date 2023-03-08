macro (append_source path out)
    file (
            GLOB_RECURSE
            sources
            ${path}/*.h
            ${path}/*.hpp
            ${path}/*.c
            ${path}/*.cpp
            ${path}/*.s
    )

    list (APPEND ${out} ${sources})
endmacro()