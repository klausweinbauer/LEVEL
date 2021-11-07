target_sources(${PROJECT_NAME} PRIVATE    
    #${CMAKE_CURRENT_SOURCE_DIR}/DENMService.hpp
    #${CMAKE_CURRENT_SOURCE_DIR}/DENMService.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/DENMContainer.hpp
    #${CMAKE_CURRENT_SOURCE_DIR}/DENMContainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DENMReceiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DENMReceiver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DENMTransmitter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DENMTransmitter.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(DENM)