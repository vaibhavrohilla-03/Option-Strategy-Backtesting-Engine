find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

file(GLOB BINDING_SOURCES "*.cpp")

pybind11_add_module(py_backtester ${BINDING_SOURCES})

target_link_libraries(py_backtester PRIVATE ${CMAKE_PROJECT_NAME})

set_target_properties(py_backtester PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/strategies"
)

set(STUBGEN_DIR "${CMAKE_SOURCE_DIR}/thirdparty/pybind11-stubgen")

add_custom_command(TARGET py_backtester POST_BUILD
    
    COMMAND ${CMAKE_COMMAND} -E env 
        "PYTHONPATH=$<TARGET_FILE_DIR:py_backtester>\;${STUBGEN_DIR}"
        
    ${Python_EXECUTABLE} -m pybind11_stubgen py_backtester -o "${CMAKE_SOURCE_DIR}/strategies"
    
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)