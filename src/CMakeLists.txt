add_subdirectory(dxfw)

if(BUILD_TESTS)
	add_subdirectory(dxfw-tests)
endif()

if(BUILD_SAMPLES)
	add_subdirectory(dxfw-samples)
endif()
