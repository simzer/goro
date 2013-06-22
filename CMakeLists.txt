cmake_minimum_required (VERSION 2.6)
cmake_policy(SET CMP0009 NEW)
project (goro)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
file(GLOB_RECURSE sourcelist src src/*.c)
add_executable (goro ${sourcelist})
target_link_libraries(goro m)

