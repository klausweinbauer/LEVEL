cmake_minimum_required(VERSION 3.7.2)

project(CAM_DENM_Example)

add_executable(${PROJECT_NAME} ert_main.c)
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.c
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.h
)

target_link_libraries(${PROJECT_NAME} c2xcam c2xdenm)