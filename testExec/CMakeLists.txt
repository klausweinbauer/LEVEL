cmake_minimum_required(VERSION 3.7.2)

project(test-exec)

add_executable(${PROJECT_NAME} main.cpp)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../c2xcam/src ${CMAKE_CURRENT_BINARY_DIR}/c2xcam)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} gtest gtest_main c2xcam)