# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.27)

project ("adventofcode2023")

# Include sub-projects.
add_subdirectory ("day01")
add_subdirectory ("day02")
add_subdirectory ("day03")
