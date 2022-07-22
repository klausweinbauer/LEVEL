target_sources(${LEVEL_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Exception.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/level_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NetworkException.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})