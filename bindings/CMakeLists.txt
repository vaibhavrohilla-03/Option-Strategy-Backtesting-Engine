find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

pybind11_add_module(py_backtester bindings.cpp)

target_link_libraries(py_backtester PRIVATE ${CMAKE_PROJECT_NAME})

set_target_properties(py_backtester PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/strategies"
)