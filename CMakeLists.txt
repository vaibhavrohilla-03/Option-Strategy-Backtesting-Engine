cmake_minimum_required(VERSION "3.19.2")

project(OptionStrategyBacktestingEngine)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(thirdparty)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC "RESOURCES_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/resources/\"")
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH = "./resources/")


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Boost::program_options
    Boost::filesystem
    Boost::random
    Boost::date_time
    Boost::system
)

