target_sources(${LEVEL_TARGET_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/EncoderTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/NetworkAccessLayerTests.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main)