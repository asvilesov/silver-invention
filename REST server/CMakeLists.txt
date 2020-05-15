# Require CMAKE 3.7 or higher
cmake_minimum_required(VERSION 3.7)

# Project name
project(pa7)

# Build for C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Turn on any compiler-specific flags
if (WIN32)
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
   if ("$ENV{RELEASE}" STREQUAL "ON")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
	endif()
	if ("$ENV{SANITIZE}" STREQUAL "ON")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	endif()
endif()

# Any source files in this directory
set(SOURCE_FILES Main.cpp)

# Where any include files are
include_directories(src)

# Subdirectories to build
add_subdirectory(src)

# Name of executable
add_executable(main ${SOURCE_FILES})

# Link main vs the source library
target_link_libraries(main src)
