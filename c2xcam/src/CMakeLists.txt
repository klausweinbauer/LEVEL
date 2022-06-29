target_sources(${C2X_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMTransmitter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Factory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IDatabase.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InMemoryDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InMemoryDatabase.hpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../common ${CMAKE_CURRENT_BINARY_DIR}/common)

target_include_directories(${C2X_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${C2X_TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_subdirectory(CAM)