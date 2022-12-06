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

include(FetchContent)

cmake_minimum_required(VERSION 3.25)

set(VCPKG_BUILD_TYPE release)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")

if (DEFINED ANDROID_ABI)
    if (DEFINED VCPKG_CHAINLOAD_TOOLCHAIN_FILE)
        message(FATAL "Cannot specify an external chainload toolchain if building for Android.")
    endif()

    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE $ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake)
endif()

project(nakama-cpp-cocos2d-x LANGUAGES CXX)

FetchContent_Declare(
  cocos2d-x
  GIT_REPOSITORY https://github.com/heroiclabs/cocos2d-x ## TODO switch to cocos mainline once they accept the PR
  GIT_TAG        2b58dc8aa85573908e230dfef6dcb48408a0010a
  GIT_PROGRESS   TRUE
  USES_TERMINAL_DOWNLOAD TRUE
)

# populate variables but don't add subdirectory until we download the remaining dependencies
FetchContent_Populate(cocos2d-x)

message("SYSTEM NAME ${CMAKE_SYSTEM_NAME}")

execute_process(COMMAND ${CMAKE_COMMAND} -E echo "yes" # accept metal support
     COMMAND "python" "download-deps.py"
     WORKING_DIRECTORY ${cocos2d-x_SOURCE_DIR}
     COMMAND_ECHO STDOUT ECHO_OUTPUT_VARIABLE ECHO_ERROR_VARIABLE COMMAND_ERROR_IS_FATAL ANY)


# hack -- there is a bug in cocos that brings in glfw3 because Linux is on even though Android is also on.
# we can't fix it because it's build code in a separate repo other than cocos2d-x that they have.
# glfw3 cannot be built on Android
message("LINUX ? ${LINUX}")
add_subdirectory(${cocos2d-x_SOURCE_DIR} ${cocos2d-x_BINARY_DIR})


 # tell CMake it's okay that we override the cocos options from parent directory
cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

## cocos-2d-x flags
set(BUILD_CPP_TESTS CACHE INTERNAL OFF)
set(BUILD_LUA_LIBS CACHE INTERNAL OFF)
set(BUILD_CPP_TEMPLATE CACHE INTERNAL OFF)
set(BUILD_EDITOR_COCOSTUDIO CACHE INTERNAL OFF)
set(BUILD_EDITOR_SPINE CACHE INTERNAL OFF)
set(BUILD_EXTENSIONS CACHE INTERNAL OFF)

### cocos is not installable so we resort to an add_subdirectory workaround.

add_subdirectory($ENV{VCPKG_ROOT}/buildtrees/cocos2d-x/cocos2d-x-src $ENV{VCPKG_ROOT}/buildtrees/cocos2d-x/cocos2d-x-build-${TARGET_TRIPLET})

include(CMakePackageConfigHelpers)

# no ZERO_CHECK target for Xcode
set(CMAKE_SUPPRESS_REGENERATION true)

install(IMPORTED_RUNTIME_ARTIFACTS nakama-sdk
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/nakama-third-party DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_subdirectory(example)
