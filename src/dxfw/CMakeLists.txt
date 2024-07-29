cmake_minimum_required(VERSION 3.5)

project(dxfw VERSION 0.1 LANGUAGES C)

set(PROJECT_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw")

set(PROJECT_SRCS
  ${PROJECT_INCLUDE_DIR}/dxfw/dxfw.h
  ${PROJECT_INCLUDE_DIR}/dxfw/enums.h
  ${PROJECT_INCLUDE_DIR}/dxfw/macros.h
  ${PROJECT_SOURCE_DIR}/dxfw-internal-macros.h
  ${PROJECT_SOURCE_DIR}/dxfw-internal-structs.h
  ${PROJECT_SOURCE_DIR}/dxfw-internal.h
  ${PROJECT_SOURCE_DIR}/dxfw.c
  ${PROJECT_SOURCE_DIR}/event-manager.c
  ${PROJECT_SOURCE_DIR}/helpers.c
  ${PROJECT_SOURCE_DIR}/keyboard.c
  ${PROJECT_SOURCE_DIR}/window.c
)

include_directories("${PROJECT_INCLUDE_DIR}")

add_library(${PROJECT_NAME} STATIC "${PROJECT_SRCS}")

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE")
