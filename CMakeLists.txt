cmake_minimum_required(VERSION 3.4)

project(DXFW)

set(ROOT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ROOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include("gtest.cmake")

add_subdirectory(src)
