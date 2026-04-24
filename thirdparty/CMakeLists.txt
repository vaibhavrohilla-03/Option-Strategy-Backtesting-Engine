add_subdirectory(pybind11)

set(DATABENTO_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(DATABENTO_ENABLE_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(databento-cpp)