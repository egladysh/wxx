cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(wxx LANGUAGES CXX)

include(GNUInstallDirs)

if( APPLE )
	set( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -Wall" )
else()
	set( CMAKE_CXX_FLAGS "-std=c++11 -Wall -pthread" )
endif()

include_directories("include")

file(GLOB src src/*.cpp src/*.h include/${PROJECT_NAME}/*.h)

add_library(${PROJECT_NAME} ${src})

# Attach header directory information
# to the targets for when we are part of a parent build (ie being pulled
# in via add_subdirectory() rather than being a standalone build).
target_include_directories(${PROJECT_NAME} INTERFACE "include")

# Install rules
install(TARGETS ${PROJECT_NAME} 
		LIBRARY DESTINATION "lib"
		ARCHIVE DESTINATION "lib"
		RUNTIME DESTINATION "bin"
	)
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include) 

add_executable(hello sample/hello.cpp)
target_link_libraries(hello ${PROJECT_NAME})
