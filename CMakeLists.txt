cmake_minimum_required(VERSION 3.4)

project(DXFW)

set(ROOT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(ROOT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include("cmake/cmocka.cmake")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(MSVC)
  foreach(lang C CXX)
    if("${CMAKE_${lang}_FLAGS}" MATCHES "/W[0-4]")
      STRING(REGEX REPLACE "/W[1-3]" "/W4 /WX" CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS}")
    else("${CMAKE_${lang}_FLAGS}" MATCHES "/W[1-3]")
      SET(CMAKE_${lang}_FLAGS "${CMAKE_${lang}_FLAGS} /W4 /WX")
    endif()
  endforeach()
endif()

add_subdirectory(src)
