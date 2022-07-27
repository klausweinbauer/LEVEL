target_sources(${LEVEL_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/level_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Syscall.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})