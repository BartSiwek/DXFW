cmake_minimum_required(VERSION 3.4)

project(dxfw-tests VERSION 0.1 LANGUAGES CXX)

set(PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(PROJECT_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/dxfw-tests.cpp
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}" "${GTEST_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw gtest)
