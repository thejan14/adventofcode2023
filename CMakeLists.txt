# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.27)

project ("adventofcode2023" LANGUAGES CXX)

add_library(aoc INTERFACE)
set_target_properties(aoc PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(aoc INTERFACE cxx_std_23)
target_include_directories(aoc INTERFACE "lib")

# Include sub-projects.
add_subdirectory ("day01")
add_subdirectory ("day02")
add_subdirectory ("day03")
add_subdirectory ("day04")
add_subdirectory ("day05")
add_subdirectory ("day06")
add_subdirectory ("day07")
add_subdirectory ("day08")
add_subdirectory ("day09")
add_subdirectory ("day10")
add_subdirectory ("day11")
add_subdirectory ("day12")
add_subdirectory ("day13")
add_subdirectory ("day14")
add_subdirectory ("day15")
add_subdirectory ("day16")
add_subdirectory ("day17")
