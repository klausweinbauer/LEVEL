cmake_minimum_required(VERSION 3.7.2)

project(c2xtest)

add_executable(${PROJECT_NAME} main.cpp c2xcam-test.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main c2xcam)