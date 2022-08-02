add_subdirectory(common)
add_subdirectory(msg)
add_subdirectory(database)
add_subdirectory(cam)
add_subdirectory(denm)
add_subdirectory(di)
add_subdirectory(api)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)