cmake_minimum_required(VERSION 3.4)

project(dxfw-tests VERSION 0.1 LANGUAGES C)

set(PROJECT_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw-tests")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/main.c
  ${PROJECT_SOURCE_DIR}/dxfw-tests.c
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw libcmocka)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE"	)
