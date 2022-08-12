target_sources(${LEVEL_TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/level_common.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/POTI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Syscall.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UDPSocket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ValueConverter.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})