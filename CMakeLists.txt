cmake_minimum_required(VERSION 3.2)
project(pathfinder)

file(GLOB_RECURSE pathfinder_SOURCES src/*.c)
set_source_files_properties(${pathfinder_SOURCES} PROPERTIES LANGUAGE CXX)
add_library(pathfinder ${pathfinder_SOURCES})
target_include_directories(pathfinder PUBLIC include)

function(pathfinder_example name)
    add_executable(pathfinder_example_${name} examples/${name}.c)
    target_include_directories(pathfinder_example_${name} PUBLIC include)
    target_link_libraries(pathfinder_example_${name} pathfinder)
    set_target_properties(pathfinder_example_${name} PROPERTIES EXCLUDE_FROM_ALL TRUE EXCLUDE_FROM_DEFAULT_BUILD TRUE)
endfunction()

pathfinder_example(basic)
pathfinder_example(swerve)
pathfinder_example(tank)
