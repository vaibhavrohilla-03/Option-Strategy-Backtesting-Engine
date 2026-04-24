cmake_minimum_required(VERSION "3.20")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OptionStrategyBacktestingEngine)

# if(MINGW)
#     set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
# endif()


find_package(OpenSSL REQUIRED)
find_package(zstd CONFIG REQUIRED)


if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    #add_compile_options(/std:c++20)
endif()

add_subdirectory(thirdparty)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(${CMAKE_PROJECT_NAME} STATIC ${MY_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    zstd::libzstd_shared                    
    databento::databento          
    nlohmann_json::nlohmann_json  
    httplib::httplib)

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

add_subdirectory(bindings)
