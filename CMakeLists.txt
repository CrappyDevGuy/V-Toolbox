cmake_minimum_required(VERSION 3.0)

project(myproject)

include_directories(src/${YOUR_DIRECTORY})
include_directories(include/${YOUR_DIRECTORY})

file(GLOB_RECURSE SRCS src/*)
file(GLOB_RECURSE HEADERS include/*)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myproject ${SRCS} ${HEADERS})
