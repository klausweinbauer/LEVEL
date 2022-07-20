set(LEVEL_TARGET_NAME LEVEL_Test)

add_executable(${LEVEL_TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/level_test.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(common)
add_subdirectory(cam)
add_subdirectory(denm)

target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main level)

gtest_discover_tests(${LEVEL_TARGET_NAME})