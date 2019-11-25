cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.1)

project (sFVM LANGUAGES CXX)

include_directories(BEFORE SYSTEM ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
add_subdirectory(source)


