cmake_minimum_required(VERSION 3.4)

project(triangle VERSION 0.1 LANGUAGES CXX)

set(PROJECT_INCLUDE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/triangle")
set(PROJECT_SOURCE_DIR "${ROOT_SOURCE_DIR}/dxfw-samples/triangle")

set(PROJECT_SRCS
  ${PROJECT_SOURCE_DIR}/main.cpp
)

set(PROJECT_ASSETS
  ${PROJECT_SOURCE_DIR}/shaders.hlsl
)

include_directories("${PROJECT_INCLUDE_DIR}" "${ROOT_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "${PROJECT_SRCS}")

target_link_libraries(${PROJECT_NAME} dxfw d3d11.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "_UNICODE;UNICODE")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup")

foreach(CURRENT_ASSET ${PROJECT_ASSETS})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   	   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CURRENT_ASSET}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()



