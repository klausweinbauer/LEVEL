cmake_minimum_required(VERSION 3.16.3)

project(level-example VERSION 0.1.0)

add_executable(${PROJECT_NAME} main.c)

add_library(level SHARED IMPORTED)
set_target_properties(level PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/liblevel.so)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME} level)