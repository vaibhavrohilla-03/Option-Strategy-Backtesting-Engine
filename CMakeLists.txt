cmake_minimum_required(VERSION "3.20")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OptionStrategyBacktestingEngine)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

add_library(cpp-httplib INTERFACE)
target_include_directories(cpp-httplib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")


if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    #add_compile_options(/std:c++20)
endif()

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${CMAKE_PROJECT_NAME} STATIC ${MY_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC 
    nlohmann_json
    cpp-httplib)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE 
    ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/Utils/"
    )

#target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC "RESOURCES_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\"")
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH = "./resources/")

add_subdirectory(scripts)
add_subdirectory(strategies)
add_subdirectory(thirdparty)
add_subdirectory(bindings)
