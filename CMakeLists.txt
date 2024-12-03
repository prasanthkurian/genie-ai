cmake_minimum_required(VERSION 3.10)
project(genie VERSION 1.0)
include_directories(${CMAKE_SOURCE_DIR}/src)
set(SOURCES
	src/main.cpp
	src/genie.cpp
   )

add_executable(genie ${SOURCES})
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
target_include_directories(genie PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(genie PUBLIC m curl json-c)
