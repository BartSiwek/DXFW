cmake_minimum_required(VERSION 3.4)

project(dxfw-tests VERSION 0.1 LANGUAGES C)

include("${CMAKE_EXTRAS}/cmocka.cmake")

set(PROJECT_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw-tests")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/dxfw-tests.h
  ${PROJECT_SOURCE_DIR}/dxfw-tests.c
  ${PROJECT_SOURCE_DIR}/window-tests.c
  ${PROJECT_SOURCE_DIR}/keyboard-tests.c
  ${PROJECT_SOURCE_DIR}/event-manager-tests.c
  ${PROJECT_SOURCE_DIR}/mocks.c
  ${PROJECT_SOURCE_DIR}/os-mocks.c
  ${PROJECT_SOURCE_DIR}/main.c
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw libcmocka)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE")

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ignore:4217")

get_target_property(CMOCKA_DLL_LOCATION libcmocka "IMPORTED_LOCATION")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMOCKA_DLL_LOCATION}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)

string(REPLACE "user32.lib " "" CMAKE_C_STANDARD_LIBRARIES ${CMAKE_C_STANDARD_LIBRARIES})
string(REPLACE "user32.lib " "" CMAKE_C_STANDARD_LIBRARIES_INIT ${CMAKE_C_STANDARD_LIBRARIES_INIT})
