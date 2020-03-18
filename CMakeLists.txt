cmake_minimum_required(VERSION 3.13)
project(heatmap)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -g")

include_directories(src)
add_subdirectory(src)
add_subdirectory(example)

