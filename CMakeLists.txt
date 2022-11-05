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

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")

project(nakama-cpp-cocos2d-x LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(nakama-sdk)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*")
file(GLOB HEADERS include/*)

add_library(nakama-cpp-cocos2d-x SRCS)
target_include_directories(nakama-cpp-cocos2d-x PUBLIC HEADERS)
target_link_libraries(nakama-cpp-cocos2d-x PUBLIC nakama-sdk)

FetchContent_Declare(
  cocos2d-x
  GIT_REPOSITORY https://github.com/cocos2d/cocos2d-x
  GIT_TAG        90f6542cf7fb081335f04e474b880d7ce8c445a1
)


target_link_libraries(nakama-cpp-cocos2d-x PRIVATE cocos2d-x)
