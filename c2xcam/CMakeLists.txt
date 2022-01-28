cmake_minimum_required(VERSION 3.6.0)

project(c2xcam VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED src/c2xcam.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER 
	"${CMAKE_CURRENT_SOURCE_DIR}/../include/c2xcam.h;${CMAKE_CURRENT_SOURCE_DIR}/../include/c2xcommon.h")

include(GenerateExportHeader)

add_subdirectory(src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

if(WIN32) 
	target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
endif()

install(TARGETS ${PROJECT_NAME}
    COMPONENT CppCAMLib
	RUNTIME DESTINATION lib
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	PUBLIC_HEADER DESTINATION include)