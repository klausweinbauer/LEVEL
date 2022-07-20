cmake_minimum_required(VERSION 3.16.3)

project(level VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED level.cpp)
set(LEVEL_TARGET_NAME ${PROJECT_NAME})

set_target_properties(${LEVEL_TARGET_NAME} PROPERTIES PUBLIC_HEADER 
	"${CMAKE_CURRENT_SOURCE_DIR}/../include/level.h")

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GenerateExportHeader)

add_subdirectory(cam)
add_subdirectory(msg)

if(WIN32) 
	target_link_libraries(${LEVEL_TARGET_NAME} wsock32 ws2_32)
elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE Threads::Threads)
endif()