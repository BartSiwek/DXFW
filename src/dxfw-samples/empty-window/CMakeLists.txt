cmake_minimum_required(VERSION 3.5)

project(empty-window VERSION 0.1 LANGUAGES CXX)

set(PROJECT_INCLUDE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/empty-window")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/empty-window")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/main.cpp
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
