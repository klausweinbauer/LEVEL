set(LEVEL_TARGET_NAME IntegrationTests)

set_property(GLOBAL PROPERTY LEVEL_INTEGRATION_TEST_TARGET ${LEVEL_TARGET_NAME})
set_property(GLOBAL PROPERTY LEVEL_INTEGRATION_TEST_TARGET_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${LEVEL_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_sources(${LEVEL_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/DatabaseTests.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/NetworkTests.cpp
)

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR},
  ${CMAKE_CURRENT_SOURCE_DIR}/../mocks
)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

if(WIN32) 
  target_link_libraries(${LEVEL_TARGET_NAME} gtest_main level wsock32 ws2_32)
elseif(UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE gtest_main level Threads::Threads)
endif()

gtest_discover_tests(${LEVEL_TARGET_NAME})