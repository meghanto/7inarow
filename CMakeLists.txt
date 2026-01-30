cmake_minimum_required(VERSION 3.16)

project(SevenInARowHarness LANGUAGES CXX)

option(GAME_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(gamecore
    src/core/Board.cpp
    src/core/Edges.cpp
    src/core/Game.cpp
    src/metrics/Potential.cpp
    src/util/Cli.cpp
    src/util/Format.cpp
)

target_include_directories(gamecore PUBLIC src)

set(COMMON_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
)

set(GCC_WARNINGS
    -Wshadow
    -Wconversion
)

set(CLANG_WARNINGS
    -Wshadow
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(gamecore PRIVATE ${COMMON_WARNINGS} ${GCC_WARNINGS})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gamecore PRIVATE ${COMMON_WARNINGS} ${CLANG_WARNINGS})
else()
    target_compile_options(gamecore PRIVATE ${COMMON_WARNINGS})
endif()

if (GAME_WARNINGS_AS_ERRORS)
    target_compile_options(gamecore PRIVATE -Werror)
endif()

add_executable(game src/main.cpp)

target_link_libraries(game PRIVATE gamecore)

target_include_directories(game PRIVATE src)

add_executable(game_tests
    tests/test_main.cpp
    tests/test_edges.cpp
    tests/test_potential.cpp
)

target_link_libraries(game_tests PRIVATE gamecore)

target_include_directories(game_tests PRIVATE src)

add_test(NAME game_tests COMMAND game_tests)

enable_testing()