cmake_minimum_required(VERSION 3.0)

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

project(myproject)

include_directories(src/${YOUR_DIRECTORY})
include_directories(include/${YOUR_DIRECTORY})
include_directories(dependencies/${YOUR_DIRECTORY})
include_directories(samples/${YOUR_DIRECTORY})

file(GLOB_RECURSE SRCS src/* samples/* dependencies/*)
file(GLOB_RECURSE HEADERS include/* dependencies/*)

find_package(glfw3 REQUIRED)
if(GLFW_FOUND)
	include_directories(${GLFW_INCLUDE_DIR})
  message(STATUS "Include GLFW Library Path : " ${GLFW_INCLUDE_DIRS})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myproject ${SRCS} ${HEADERS} dependencies/volk.h)

target_compile_definitions(myproject PRIVATE VULKAN_NO_PROTOTYPES)

target_link_libraries(myproject ${GLFW_LIBRARIES})
target_link_libraries(myproject ${CMAKE_DL_LIBS})
