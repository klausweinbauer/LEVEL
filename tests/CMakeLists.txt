cmake_minimum_required(VERSION 3.6.0)

project(c2xtest)

add_executable(${PROJECT_NAME} main.cpp c2xcam-test.cpp c2xdenm-test.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main c2xcam c2xdenm)