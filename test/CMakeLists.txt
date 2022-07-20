################################
# API Test 
################################

set(LEVEL_TARGET_NAME LEVEL_APITest)

add_executable(${LEVEL_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/level_test.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main level)

gtest_discover_tests(${LEVEL_TARGET_NAME})

################################
# Unit Test 
################################

set(LEVEL_TARGET_NAME LEVEL_UnitTest)

add_executable(${LEVEL_TARGET_NAME})

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(common)
add_subdirectory(cam)
add_subdirectory(denm)

target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main)

gtest_discover_tests(${LEVEL_TARGET_NAME})