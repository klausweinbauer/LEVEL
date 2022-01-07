cmake_minimum_required(VERSION 3.7.2)

project(c2xcli)

add_executable(${PROJECT_NAME} main.cpp)

FIND_PACKAGE(Boost COMPONENTS program_options filesystem REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} c2xcam c2xdenm)