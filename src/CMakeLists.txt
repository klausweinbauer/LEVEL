add_subdirectory(common)
add_subdirectory(msg)
add_subdirectory(cam)
add_subdirectory(di)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)