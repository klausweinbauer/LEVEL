
set(C2X_TARGET_NAME c2xcam_UnitTest)

add_executable(${C2X_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/EncoderTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/UDPUtilityTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/NetworkAccessLayerTests.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src/CAM ${CMAKE_CURRENT_BINARY_DIR}/src/CAM)

target_sources(${C2X_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/InMemoryDatabase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/Encoder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../src/UDPNetworkAccessLayer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../common/UDPSocket.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../../common/PacketReceiver.cpp
)

target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/)
target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../common/)
target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include/)

target_link_libraries(${C2X_TARGET_NAME} gtest_main gmock_main)

gtest_discover_tests(${C2X_TARGET_NAME})