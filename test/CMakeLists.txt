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

add_executable(${LEVEL_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/UDPUtilityTests.cpp
)

add_subdirectory(cam)

gtest_discover_tests(${LEVEL_TARGET_NAME})