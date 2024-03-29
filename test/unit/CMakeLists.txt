set(LEVEL_TARGET_NAME UnitTests)

set_property(GLOBAL PROPERTY LEVEL_UNIT_TEST_TARGET ${LEVEL_TARGET_NAME})
set_property(GLOBAL PROPERTY LEVEL_UNIT_TEST_TARGET_PATH ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${LEVEL_TARGET_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/level_test.cpp
)

if(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage --coverage -O0")
endif()

target_include_directories(${LEVEL_TARGET_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR},
  ${CMAKE_SOURCE_DIR}/include,
  ${CMAKE_CURRENT_SOURCE_DIR}/../mocks)

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(common)
add_subdirectory(database)
add_subdirectory(cam)
add_subdirectory(denm)
add_subdirectory(api)

if(WIN32) 
  target_link_libraries(${LEVEL_TARGET_NAME} gtest_main gmock_main level wsock32 ws2_32)
elseif(UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE gtest_main gmock_main level Threads::Threads)
endif()

gtest_discover_tests(${LEVEL_TARGET_NAME})