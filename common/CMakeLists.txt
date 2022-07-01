message("Common: " ${C2X_TARGET_NAME})

target_sources(${C2X_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/c2xcommon.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VectorBuffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VectorBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketReceiver.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Exception.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/NetworkException.hpp 
)

target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${C2X_TARGET_NAME} PRIVATE &{CMAKE_SOURCE_DIR}/include/)