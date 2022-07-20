cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb3")

project(level VERSION 0.1.0)

option(BUILD_TESTS "include tests in build" ON)
option(COVERAGE "include code coverage target in build" ON)

################################
# LEVEL 
################################

add_library(${PROJECT_NAME} SHARED src/level.cpp)
set(LEVEL_TARGET_NAME ${PROJECT_NAME})

set_target_properties(${LEVEL_TARGET_NAME} PROPERTIES PUBLIC_HEADER 
	"${CMAKE_CURRENT_SOURCE_DIR}/../include/level.h")

include(GenerateExportHeader)

add_subdirectory(src)

if(WIN32) 
	target_link_libraries(${LEVEL_TARGET_NAME} wsock32 ws2_32)
elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(${LEVEL_TARGET_NAME} PRIVATE Threads::Threads)
endif()

################################
# GTest
################################
if (BUILD_TESTS)
    message("[INFO] Tests enabled")

    include(FetchContent)
    FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        15460959cbbfa20e66ef0b5ab497367e47fc0a04 # release-1.12.0
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    enable_testing()

    add_subdirectory(test)
else()
    message("[INFO] Tests disabled")
endif()

################################
# Code Coverage
################################

# ===== Modified import START =====
# Functions imported and modified from https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake
# Accessed: July 20, 2022

# Copyright (c) 2012 - 2017, Lars Bilke
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

function(append_coverage_compiler_flags)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${COVERAGE_COMPILER_FLAGS}" PARENT_SCOPE)
    message(STATUS "Appending code coverage compiler flags: ${COVERAGE_COMPILER_FLAGS}")
endfunction() # append_coverage_compiler_flags

# Defines a target for running and collection code coverage information
# Builds dependencies, runs the given executable and outputs reports.
# NOTE! The executable should always have a ZERO as exit code otherwise
# the coverage generation will not complete.
#
# setup_target_for_coverage_gcovr_html(
#     NAME ctest_coverage                    # New target name
#     EXECUTABLE ctest -j ${PROCESSOR_COUNT} # Executable in PROJECT_BINARY_DIR
#     DEPENDENCIES executable_target         # Dependencies to build first
#     BASE_DIRECTORY "../"                   # Base directory for report
#                                            #  (defaults to PROJECT_SOURCE_DIR)
#     EXCLUDE "src/dir1/*" "src/dir2/*"      # Patterns to exclude (can be relative
#                                            #  to BASE_DIRECTORY, with CMake 3.4+)
# )
# The user can set the variable GCOVR_ADDITIONAL_ARGS to supply additional flags to the
# GCVOR command.
function(setup_target_for_coverage_gcovr_html)

    set(options NONE)
    set(oneValueArgs BASE_DIRECTORY NAME)
    set(multiValueArgs EXCLUDE EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif() # NOT GCOVR_PATH

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(DEFINED Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    # Collect excludes (CMake 3.4+: Also compute absolute paths)
    set(GCOVR_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE} ${COVERAGE_EXCLUDES} ${COVERAGE_GCOVR_EXCLUDES})
        if(CMAKE_VERSION VERSION_GREATER 3.4)
            get_filename_component(EXCLUDE ${EXCLUDE} ABSOLUTE BASE_DIR ${BASEDIR})
        endif()
        list(APPEND GCOVR_EXCLUDES "${EXCLUDE}")
    endforeach()
    list(REMOVE_DUPLICATES GCOVR_EXCLUDES)

    # Combine excludes to several -e arguments
    set(GCOVR_EXCLUDE_ARGS "")
    foreach(EXCLUDE ${GCOVR_EXCLUDES})
        list(APPEND GCOVR_EXCLUDE_ARGS "-e")
        list(APPEND GCOVR_EXCLUDE_ARGS "${EXCLUDE}")
    endforeach()

    # Set up commands which will be run to generate coverage data
    # Run tests
    set(GCOVR_HTML_EXEC_TESTS_CMD
        ${Coverage_EXECUTABLE} ${Coverage_EXECUTABLE_ARGS}
    )
    # Create folder
    set(GCOVR_HTML_FOLDER_CMD
        ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${Coverage_NAME}
    )
    # Running gcovr
    set(GCOVR_HTML_CMD
        ${GCOVR_PATH} --html ${Coverage_NAME}/index.html --html-details -r ${CMAKE_SOURCE_DIR} ${GCOVR_ADDITIONAL_ARGS}
        ${GCOVR_EXCLUDE_ARGS} --object-directory=${CMAKE_BINARY_DIR}
    )

    if(CODE_COVERAGE_VERBOSE)
        message(STATUS "Executed command report")

        message(STATUS "Command to run tests: ")
        string(REPLACE ";" " " GCOVR_HTML_EXEC_TESTS_CMD_SPACED "${GCOVR_HTML_EXEC_TESTS_CMD}")
        message(STATUS "${GCOVR_HTML_EXEC_TESTS_CMD_SPACED}")

        message(STATUS "Command to create a folder: ")
        string(REPLACE ";" " " GCOVR_HTML_FOLDER_CMD_SPACED "${GCOVR_HTML_FOLDER_CMD}")
        message(STATUS "${GCOVR_HTML_FOLDER_CMD_SPACED}")

        message(STATUS "Command to generate gcovr HTML coverage data: ")
        string(REPLACE ";" " " GCOVR_HTML_CMD_SPACED "${GCOVR_HTML_CMD}")
        message(STATUS "${GCOVR_HTML_CMD_SPACED}")
    endif()

    add_custom_target(${Coverage_NAME}
        COMMAND ${GCOVR_HTML_EXEC_TESTS_CMD}
        COMMAND ${GCOVR_HTML_FOLDER_CMD}
        COMMAND ${GCOVR_HTML_CMD}

        BYPRODUCTS ${PROJECT_BINARY_DIR}/${Coverage_NAME}/index.html  # report directory
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM # Protect arguments to commands
        COMMENT "Running gcovr to produce HTML code coverage report."
    )

    # Show info where to find the report
    add_custom_command(TARGET ${Coverage_NAME} POST_BUILD
        COMMAND ;
        COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report."
    )

endfunction() # setup_target_for_coverage_gcovr_html

# ===== Modified import END =====

if (COVERAGE)
    message("[INFO] Code coverage enabled")
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(
            NAME LEVEL_Coverage
            EXECUTABLE LEVEL_Test
            EXCLUDE "${CMAKE_SOURCE_DIR}/src/msg/*" "${CMAKE_SOURCE_DIR}/build/*" "${CMAKE_SOURCE_DIR}/test/*")
else()
    message("[INFO] Code coverage disabled")
endif()