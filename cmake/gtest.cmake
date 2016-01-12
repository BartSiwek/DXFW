# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
  gtestandgmock
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG master
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtestandgmock
  INSTALL_COMMAND ""
)

# Get the binary and source dirs
ExternalProject_Get_Property(gtestandgmock source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtestandgmock)
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtestandgmock)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/${CMAKE_CFG_INTDIR}/gtest.lib"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/googletest/include")

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/${CMAKE_CFG_INTDIR}/gmock.lib"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/googlemock/include")

