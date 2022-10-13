add_subdirectory(common)
add_subdirectory(msg)
add_subdirectory(database)
add_subdirectory(cam)
add_subdirectory(denm)

target_sources(${LEVEL_TARGET_NAME} PRIVATE    
    ${CMAKE_CURRENT_SOURCE_DIR}/level.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)