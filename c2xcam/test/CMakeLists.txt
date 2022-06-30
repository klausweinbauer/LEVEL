
set(C2X_TARGET_NAME c2xcam_UnitTest)

add_executable(${C2X_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTests.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src/CAM ${CMAKE_CURRENT_BINARY_DIR}/src/CAM)

target_sources(${C2X_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/InMemoryDatabase.cpp
)

target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../common/)
target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/)

target_link_libraries(${C2X_TARGET_NAME} gtest_main gmock_main)

gtest_discover_tests(${C2X_TARGET_NAME})