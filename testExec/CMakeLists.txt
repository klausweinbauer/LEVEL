cmake_minimum_required(VERSION 3.7.2)

project(test-exec)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../c2xcam/src/CAM ${CMAKE_CURRENT_BINARY_DIR}/CAM)

target_link_libraries(${PROJECT_NAME} gtest gtest_main c2xcam)