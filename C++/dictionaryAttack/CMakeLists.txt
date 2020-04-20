# Require CMAKE 3.1 or higher
cmake_minimum_required(VERSION 3.1)

# Project name
project(dictionaryAttack)

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

# setting binary directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Find TBB
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(TBB REQUIRED)

# Any source files in this directory
set(SOURCE_FILES Main.cpp)

# Where any include files are
include_directories(src)
include_directories(${TBB_INCLUDE_DIRS})

# Subdirectories to build
add_subdirectory(src)

# Name of executable
add_executable(main ${SOURCE_FILES})

# Link main vs the source library
target_link_libraries(main src tbb)
if (WIN32)
   add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${TBB_DLL_PATH}/tbb.dll"
        $<TARGET_FILE_DIR:main>)
   add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${TBB_DLL_PATH}/tbb_debug.dll"
        $<TARGET_FILE_DIR:main>)
endif()
