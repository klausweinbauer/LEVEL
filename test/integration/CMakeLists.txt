set(LEVEL_TARGET_NAME LEVEL_Integration_Tests)

add_executable(${LEVEL_TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR},
  ${CMAKE_CURRENT_SOURCE_DIR}/../mocks
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(database)

if(WIN32) 
  target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main level wsock32 ws2_32)
elseif(UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE gtest_main gmock_main level Threads::Threads)
endif()

gtest_discover_tests(${LEVEL_TARGET_NAME})