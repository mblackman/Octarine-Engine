# Helper to declare an internal STATIC library with the engine's standard compile
# options + warnings. Mirrors the compiler-flag block that used to live in the root
# CMakeLists.txt against a single OctarineEngine target.
#
# Usage:
#   octarine_library(<name> <source> [<source> ...])
#
# Public include directory defaults to ${CMAKE_SOURCE_DIR}/src so consumers can
# reach any header under src/. Feature defines (OCTARINE_WITH_IMGUI etc.) come
# from octarine_core's PUBLIC compile_definitions; consumers pick them up
# transitively, so per-target plumbing isn't needed here.

function(octarine_library NAME)
    set(_sources ${ARGN})
    add_library(${NAME} STATIC ${_sources})
    set_target_properties(${NAME} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            POSITION_INDEPENDENT_CODE ON
    )
    target_include_directories(${NAME} PUBLIC
            "${CMAKE_SOURCE_DIR}/src"
            "${CMAKE_SOURCE_DIR}/libs"
    )
    if (LUA_INCLUDE_DIR)
        target_include_directories(${NAME} PUBLIC "${LUA_INCLUDE_DIR}")
    endif ()

    if (MSVC)
        target_compile_options(${NAME} PRIVATE
                /W4
                /permissive-
                /MP
                /bigobj
                /w14242
                /w14244
                /w14254
                /w44456
                /w44457
                /w44458
                /w44459
                /wd4201
                /wd5321
        )
        # Warnings-as-errors + AVX2 only outside Debug.
        target_compile_options(${NAME} PRIVATE
                $<IF:$<CONFIG:Debug>,,-WX /arch:AVX2>
        )
    else ()
        target_compile_options(${NAME} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -Wshadow
                -Wconversion
                -Wfatal-errors
        )
        if (ANDROID)
            target_compile_options(${NAME} PRIVATE
                    $<IF:$<CONFIG:Debug>,,-ffast-math>
            )
        else ()
            target_compile_options(${NAME} PRIVATE
                    $<IF:$<CONFIG:Debug>,,-Werror -march=native -ffast-math>
            )
        endif ()
        target_compile_options(${NAME} PRIVATE
                $<$<NOT:$<CONFIG:Debug>>:-ffunction-sections>
                $<$<NOT:$<CONFIG:Debug>>:-fdata-sections>
        )
    endif ()
endfunction()
