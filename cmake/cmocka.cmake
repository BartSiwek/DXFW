# Enable ExternalProject CMake module
include(ExternalProject)

# We need git
find_package(Git REQUIRED)

# Download and install Cmocka
ExternalProject_Add(
  cmocka
  GIT_REPOSITORY git://git.cryptomilk.org/projects/cmocka.git
  GIT_TAG cmocka-1.0.1
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/cmocka
  PATCH_COMMAND "${GIT_EXECUTABLE}" apply "${CMAKE_CURRENT_LIST_DIR}/cmocka.vs2015.patch"
  INSTALL_COMMAND ""
)

# Get the binary and source dirs
ExternalProject_Get_Property(cmocka source_dir binary_dir)

# Create a libcmocka target to be used as a dependency by test programs
add_library(libcmocka IMPORTED SHARED GLOBAL)
add_dependencies(libcmocka cmocka)

# Set libcmocka properties
set_target_properties(libcmocka PROPERTIES
  "IMPORTED_IMPLIB" "${binary_dir}/src/${CMAKE_CFG_INTDIR}/cmocka.lib"
  "IMPORTED_LOCATION" "${binary_dir}/src/${CMAKE_CFG_INTDIR}/cmocka.dll"
)
include_directories("${source_dir}/include")
