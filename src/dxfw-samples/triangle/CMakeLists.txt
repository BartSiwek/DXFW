cmake_minimum_required(VERSION 3.4)

project(triangle VERSION 0.1 LANGUAGES CXX)

include("${CMAKE_EXTRAS}/directx.cmake")

if(NOT DEFINED DirectX_D3D11_FOUND)
	message(WARNING "Direct3D 11 not found. The Triangle sample will not be built")
	return()
endif()

set(PROJECT_INCLUDE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/triangle")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/triangle")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/main.cpp
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")
