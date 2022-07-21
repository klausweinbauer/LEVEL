target_sources(${LEVEL_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMData.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IDatabase.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InMemoryDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InMemoryDatabase.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IEncoder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Encoder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/INetworkInterface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketBasedNAC.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SocketBasedNAC.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})