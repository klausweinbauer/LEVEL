target_sources(${LEVEL_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/CABasicService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CAMWrapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VehicleFrequencyManager.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})