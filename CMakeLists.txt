cmake_minimum_required(VERSION 3.7.2)

project(c2xlib)

add_subdirectory(c2xcam)
add_subdirectory(c2xdenm)
add_subdirectory(tests)
add_subdirectory(testExec)

################################
# GTest
################################
add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_test(c2xtest tests/c2xtest)