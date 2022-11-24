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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(FetchContent)

## cocos-2d-x flags
set(BUILD_CPP_TESTS OFF)
set(BUILD_LUA_LIBS OFF)
set(BUILD_CPP_TEMPLATE OFF)
set(BUILD_EDITOR_COCOSTUDIO OFF)
set(BUILD_EDITOR_SPINE OFF)
set(BUILD_EXTENSIONS OFF)

FetchContent_Declare(
  cocos2d-x
  GIT_REPOSITORY https://github.com/heroiclabs/cocos2d-x ## TODO switch to cocos mainline once they accept the PR
  GIT_TAG        b26d115cb5a18282f96adb9a3a7fbb3608ec75a7
  GIT_PROGRESS   TRUE
  USES_TERMINAL_DOWNLOAD TRUE
)

FetchContent_MakeAvailable(cocos2d-x)

execute_process(COMMAND ${CMAKE_COMMAND} -E echo "yes" # accept metal support
     COMMAND "python" "download-deps.py"
     WORKING_DIRECTORY ${cocos2d-x_SOURCE_DIR}
     COMMAND_ECHO STDOUT ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE COMMAND_ERROR_IS_FATAL ANY)

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

target_link_libraries(nakama-cpp-cocos2d-x PRIVATE cocos2d PUBLIC nakama-sdk)

## we install but can't export because cocos2d isn't setup to be installable in a relocatable way.
install(TARGETS
    nakama-cpp-cocos2d-x
    EXPORT nakama-cpp-cocos2d-x-export
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDE_DIR}
)

install(IMPORTED_RUNTIME_ARTIFACTS nakama-sdk
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


if (NAKAMA_COCOS2D_X_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
