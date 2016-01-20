cmake_minimum_required(VERSION 3.4)

project(dxfw VERSION 0.1 LANGUAGES CXX)

set(PROJECT_INCLUDE_DIR "${ROOT_INCLUDE_DIR}")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw")

set(PROJECT_SRCS
  ${PROJECT_INCLUDE_DIR}/dxfw/dxfw.h
  ${PROJECT_SOURCE_DIR}/dxfw.cpp
  ${PROJECT_INCLUDE_DIR}/dxfw/window.h
  ${PROJECT_SOURCE_DIR}/window.cpp
  ${PROJECT_INCLUDE_DIR}/dxfw/window_manager.h
  ${PROJECT_SOURCE_DIR}/window_manager.cpp
  ${PROJECT_INCLUDE_DIR}/dxfw/event_manager.h
  ${PROJECT_SOURCE_DIR}/event_manager.cpp  
)

include_directories("${PROJECT_INCLUDE_DIR}")

add_library(${PROJECT_NAME} STATIC "${PROJECT_SRCS}")
