cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb3")

project(LEVEL VERSION 0.1.0)

option(BUILD_TESTS "include tests in build" ON)

add_subdirectory(src)

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