cmake_minimum_required(VERSION 3.7.2)

project(c2xlib)

add_subdirectory(c2xcam)
add_subdirectory(c2xdenm)
add_subdirectory(testExec)

if (UNIX) 
################################
# GTest
################################
add_subdirectory(tests)
add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_test(c2xtest tests/c2xtest)
endif()


set(MATLAB_LIB_FILE_DIR matlab/c2xlib)
file(GLOB MATLAB_LIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${MATLAB_LIB_FILE_DIR}/*")
install(FILES ${MATLAB_LIB_FILES} 
        DESTINATION ${MATLAB_LIB_FILE_DIR}
        COMPONENT MatlabLib)

set(MATLAB_EXAMPLES_FILE_DIR matlab/examples)
file(GLOB MATLAB_EXAMPLE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${MATLAB_EXAMPLES_FILE_DIR}/*")
install(FILES ${MATLAB_EXAMPLE_FILES} 
        DESTINATION ${MATLAB_EXAMPLES_FILE_DIR}
        COMPONENT MatlabExamples)

set(CPACK_GENERATOR WIX)
set(CPACK_WIX_UPGRADE_GUID "03A103F9-E286-46C5-8DBE-F07E0F1A0CEF")
set(CPACK_PACKAGE_NAME "C2XLib")
set(CPACK_PACKAGE_VENDOR "Klaus Weinbauer")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ and Matlab library for c2x communication based on CAM and DENM protocolls over UDP.")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "c2xlib")

set(CPACK_COMPONENTS_ALL CppCAMLib CppDENMLib MatlabLib MatlabExamples)
set(CPACK_COMPONENT_CPPCAMLIB_DISPLAY_NAME "CAM")
set(CPACK_COMPONENT_CPPDENMLIB_DISPLAY_NAME "DENM")
set(CPACK_COMPONENT_MATLABLIB_DISPLAY_NAME "Matlab Library")
set(CPACK_COMPONENT_MATLABEXAMPLES_DISPLAY_NAME "Matlab Examples")

set(CPACK_COMPONENT_CPPCAMLIB_DESCRIPTION "Library for sending and receiving CAM messages over UDP.")
set(CPACK_COMPONENT_CPPDENMLIB_DESCRIPTION "Library for sending and receiving DENM messages over UDP.")
set(CPACK_COMPONENT_MATLABLIB_DESCRIPTION "Contains Simulink blocks to access the underlying C++ libraries.")
set(CPACK_COMPONENT_MATLABEXAMPLES_DESCRIPTION "Adds some examples for the usage of the Simulink blocks.")

set(CPACK_COMPONENT_MATLABEXAMPLES_DEPENDS MatlabLib)
set(CPACK_COMPONENT_MATLABLIB_DEPENDS CppCAMLib CppDENMLib)

set(CPACK_COMPONENT_CPPCAMLIB_GROUP "CppLib")
set(CPACK_COMPONENT_CPPDENMLIB_GROUP "CppLib")
set(CPACK_COMPONENT_GROUP_CPPLIB_DISPLAY_NAME "C++ Libraries")
set(CPACK_COMPONENT_GROUP_CPPLIB_DESCRIPTION "Includes C++ Libraries for CAM and DENM messageing service over UDP.")

include(CPack)