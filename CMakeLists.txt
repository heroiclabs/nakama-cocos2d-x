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

option(NAKAMA_COCOS2D_X_BUILD_EXAMPLE ON)
set(VCPKG_BUILD_TYPE release)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")

if (DEFINED ANDROID_ABI)
    if (DEFINED VCPKG_CHAINLOAD_TOOLCHAIN_FILE)
        message(FATAL "Cannot specify an external chainload toolchain if building for Android.")
    endif()

    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE $ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake)
endif()

project(nakama-cpp-cocos2d-x LANGUAGES CXX)

# tell CMake it's okay that we override the cocos options from parent directory
cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

### cocos is not installable so we resort to an add_subdirectory workaround.
include(CMakePackageConfigHelpers)

# no ZERO_CHECK target for Xcode
set(CMAKE_SUPPRESS_REGENERATION true)

if (NAKAMA_COCOS2D_X_BUILD_EXAMPLE)
    add_subdirectory(example)

    install(IMPORTED_RUNTIME_ARTIFACTS nakama-sdk
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    install(DIRECTORY ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/nakama-third-party DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
