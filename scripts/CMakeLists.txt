file(GLOB SCRIPT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(SCRIPT_FILE ${SCRIPT_FILES})
	
	get_filename_component(SCRIPT_NAME ${SCRIPT_FILE} NAME_WE)

	add_executable(${SCRIPT_NAME} ${SCRIPT_FILE})
	target_compile_features(${SCRIPT_NAME} PRIVATE cxx_std_20)

	set_target_properties(${SCRIPT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}"
        FOLDER "Scripts" 
    )

 message(STATUS "Added script target: ${SCRIPT_NAME}")
endforeach()