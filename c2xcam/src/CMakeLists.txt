target_sources(${PROJECT_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMTransmitter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMTransmitter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMReceiver.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(CAM)