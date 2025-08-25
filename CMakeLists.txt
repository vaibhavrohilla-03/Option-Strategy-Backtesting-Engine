cmake_minimum_required(VERSION "3.19.2")

project(OptionStrategyBacktestingEngine)

add_executable(${CMAKE_PROJECT_NAME} 
	"src/main.cpp"
	"src/Options.cpp"	
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")


