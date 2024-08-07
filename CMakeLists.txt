cmake_minimum_required(VERSION 3.5)

project(DXFW)

option(BUILD_TESTS "Builds the DXFW tests projects" ON)
option(BUILD_SAMPLES "Builds the DXFW sample projects" ON)

set(ROOT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ROOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_EXTRAS ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(MSVC)
  foreach(lang C CXX)
    if("${CMAKE_${lang}_FLAGS}" MATCHES "/W[0-4]")
      string(REGEX REPLACE "/W[1-3]" "/W4 /WX" CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS}")
    else("${CMAKE_${lang}_FLAGS}" MATCHES "/W[1-3]")
      set(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} /W4 /WX")
    endif()
  endforeach()
endif()

add_subdirectory(src)
