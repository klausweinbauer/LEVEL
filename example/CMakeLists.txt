cmake_minimum_required(VERSION 3.6.0)

project(CAM_DENM_Example)

add_executable(${PROJECT_NAME} ert_main.c)
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_ert_rtw/${PROJECT_NAME}.c
)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_ert_rtw)

target_link_libraries(${PROJECT_NAME} c2xcam c2xdenm)