cmake_minimum_required(VERSION 3.4)

project(DXFW)

set(ROOT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ROOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include("cmake/cmocka.cmake")
include("cmake/configure_msvc_runtime.cmake")

configure_msvc_runtime()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(src)
