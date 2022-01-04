target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/c2xcommon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VectorBuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VectorBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp    
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})