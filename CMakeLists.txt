cmake_minimum_required(VERSION 3.16.3)

project(level VERSION 0.1.0)

option(BUILD_TESTS "include tests in build" OFF)
option(ENABLE_VALGRIND_TESTS "Include valgrind tests in build." OFF)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb3 -fprofile-arcs -ftest-coverage --coverage -g -O0")
endif()

if(WIN32)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

################################
# LEVEL 
################################

add_library(${PROJECT_NAME} SHARED src/level.cpp)
set(LEVEL_TARGET_NAME ${PROJECT_NAME})

set_target_properties(${LEVEL_TARGET_NAME} PROPERTIES PUBLIC_HEADER 
	"${CMAKE_CURRENT_SOURCE_DIR}/../include/level.h")

include(GenerateExportHeader)

add_subdirectory(src)

if(WIN32) 
	target_link_libraries(${LEVEL_TARGET_NAME} wsock32 ws2_32)
elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE Threads::Threads)
endif()

################################
# GTest
################################
if (BUILD_TESTS)
    message("[INFO] Tests enabled")

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        15460959cbbfa20e66ef0b5ab497367e47fc0a04 # release-1.12.0
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    enable_testing()

    add_subdirectory(test)
else()
    message("[INFO] Tests disabled")
endif()