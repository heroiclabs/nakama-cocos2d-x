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

cmake_minimum_required(VERSION 3.23)

set(VCPKG_BUILD_TYPE "release")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")
project(nakama-cpp-cocos2d-x LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")

add_library(nakama-cpp-cocos2d-x SHARED ${SRCS})
include(FetchContent)

FetchContent_Declare(
  cocos-2d-x
  GIT_REPOSITORY https://github.com/cocos2d/cocos2d-x
  GIT_TAG        90f6542cf7fb081335f04e474b880d7ce8c445a1
)

## slim as much as possible, a lot more that can be done but cocos must provide options to
## allow us to do it.

# populate variables but don't add subdirectory until we download the remaining dependencies
FetchContent_Populate(cocos-2d-x)

execute_process(COMMAND ${CMAKE_COMMAND} -E echo "yes" # accept metal support
     COMMAND "python" "download-deps.py"
     WORKING_DIRECTORY ${cocos-2d-x_SOURCE_DIR}
     COMMAND_ECHO STDOUT ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE COMMAND_ERROR_IS_FATAL ANY)

add_subdirectory(${cocos-2d-x_SOURCE_DIR} ${cocos-2d-x_BINARY_DIR})

set(BUILD_EDITOR_COCOSTUDIO OFF)
set(BUILD_EDITOR_SPINE OFF)
set(BUILD_EXTENSIONS OFF)
set(BUILD_LUA_LIBS OFF)
set(BUILD_EXT_BOX2D OFF)
set(BUILD_EXT_CHIPMUNK OFF)
set(BUILD_EXT_FREETYPE2 OFF)
set(BUILD_EXT_RECAST OFF)
set(BUILD_EXT_BULLET OFF)
set(BUILD_EXT_JPEG OFF)
set(BUILD_EXT_UV OFF)
set(BUILD_EXT_TINYXML2 OFF)
set(BUILD_EXT_XXHASH OFF)
set(BUILD_EXT_XXTEA OFF)
set(BUILD_EXT_CLIPPER OFF)
set(BUILD_EXT_EDTAA3FUNC OFF)
set(BUILD_EXT_CONVERTUTF OFF)
set(BUILD_EXT_POLY2TRI OFF)
set(BUILD_LUA_LIBS OFF)

target_include_directories(nakama-cpp-cocos2d-x PUBLIC include PUBLIC ${cocos-2d-x_SOURCE_DIR})
## link against the inner cocos target
target_link_libraries(nakama-cpp-cocos2d-x PUBLIC nakama-sdk PRIVATE cocos-2d-x)
