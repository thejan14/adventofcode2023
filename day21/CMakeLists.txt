cmake_minimum_required (VERSION 3.27)

get_filename_component (CURRENT_DAY ${CMAKE_CURRENT_SOURCE_DIR} NAME)

configure_file("input.txt" "input.txt" COPYONLY)

add_executable (${CURRENT_DAY}part1 "part1.cpp")

target_link_libraries(${CURRENT_DAY}part1 PUBLIC aoc)

set_property(TARGET ${CURRENT_DAY}part1 PROPERTY CXX_STANDARD 23)
set_property(TARGET ${CURRENT_DAY}part1 PROPERTY CMAKE_CXX_STANDARD_REQUIRED  YES)
