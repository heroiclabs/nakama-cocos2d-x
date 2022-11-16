# Copyright 2022 The Nakama Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.24)

set(FETCHCONTENT_QUIET OFF)

set(VCPKG_BUILD_TYPE "release")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")
project(nakama-cpp-cocos2d-x LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(NAKAMA_COCOS2D_X_BUILD_EXAMPLE ON)

find_package(optional-lite)
find_package(nakama-sdk)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

add_library(nakama-cpp-cocos2d-x SHARED ${SRCS})

target_include_directories(nakama-cpp-cocos2d-x PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

include(FetchContent)

FetchContent_Declare(
  cocos2d-x
  GIT_REPOSITORY https://github.com/cocos2d/cocos2d-x
  GIT_TAG        90f6542cf7fb081335f04e474b880d7ce8c445a1
  GIT_PROGRESS   TRUE
  USES_TERMINAL_DOWNLOAD TRUE
)

# populate variables but don't add subdirectory until we download the remaining dependencies
FetchContent_Populate(cocos2d-x)

execute_process(COMMAND ${CMAKE_COMMAND} -E echo "yes" # accept metal support
     COMMAND "python" "download-deps.py"
     WORKING_DIRECTORY ${cocos2d-x_SOURCE_DIR}
     COMMAND_ECHO STDOUT ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE COMMAND_ERROR_IS_FATAL ANY)


add_subdirectory(${cocos2d-x_SOURCE_DIR} ${cocos2d-x_BINARY_DIR})

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/nakama-cpp-cocos2d-x-config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/nakama-cpp-cocos2d-x-config.cmake"
  INSTALL_DESTINATION share/nakama-cpp-cocos2d-x
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

if (APPLE)
    set_target_properties(nakama-cpp-cocos2d-x PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A
        PUBLIC_HEADER "${HEADERS}"
        MACOSX_FRAMEWORK_IDENTIFIER com.heroiclabs.nakama-cpp-cocos2d-x
        )
endif()

install(TARGETS
    nakama-cpp-cocos2d-x
    EXPORT nakama-cpp-cocos2d-x-export
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT nakama-cpp-cocos2d-x-export DESTINATION share/nakama-cpp-cocos2d-x)

target_link_libraries(nakama-cpp-cocos2d-x PUBLIC nakama-sdk PRIVATE cocos2d)

if (NAKAMA_COCOS2D_X_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
