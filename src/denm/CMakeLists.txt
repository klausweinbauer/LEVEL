target_sources(${LEVEL_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/DENMWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DENBasicService.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})