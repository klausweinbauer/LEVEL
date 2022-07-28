target_sources(${LEVEL_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/IEncoder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Encoder.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/INetworkInterface.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMWrapper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMWrapper.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})