cmake_minimum_required (VERSION 3.8)

get_filename_component (CURRENT_DAY ${CMAKE_CURRENT_SOURCE_DIR} NAME)

configure_file("input.txt" "input.txt" COPYONLY)

add_executable (${CURRENT_DAY}part1 "part1.cpp" )
add_executable (${CURRENT_DAY}part2 "part2.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CURRENT_DAY}part1 PROPERTY CXX_STANDARD 23)
  set_property(TARGET ${CURRENT_DAY}part2 PROPERTY CXX_STANDARD 23)
endif()
