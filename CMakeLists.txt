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
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

include(CMakePackageConfigHelpers)

# no ZERO_CHECK target for Xcode
set(CMAKE_SUPPRESS_REGENERATION true)

# republish libraries
install(IMPORTED_RUNTIME_ARTIFACTS nakama-sdk
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB LIBPROTOBUF "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libprotobuf.[a|so|dll|lib]")
file(GLOB LIBPROTOBUF_LITE "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libprotobuf-lite.[a|so|dll|lib]")
file(GLOB LIBPROTOC "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libprotoc.[a|so|dll|lib]")
file(GLOB LIBPROTOC_TOOL "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/tools/protobuf/protoc*")

install(FILES ${LIBPROTOBUF} ${LIBPROTOBUF_LITE} ${LIBPROTOC} DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${LIBPROTOC_TOOL} DESTINATION ${CMAKE_INSTALL_PREFIX}/tools/protobuf)

## republish include files
install(DIRECTORY ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/google DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*")

if (APPLE)
    set(NAKAMA_INCLUDES ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/nakama-sdk.framework/Headers)
else()
    set(NAKAMA_INCLUDES ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/nakama-cpp)
endif()

install(DIRECTORY ${NAKAMA_INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*")
install(DIRECTORY ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/nonstd DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*")

## republish config files
file(GLOB NAKAMA_CONFIGS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/nakama-sdk/*.cmake")
install(FILES ${NAKAMA_CONFIGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/nakama-sdk)

file(GLOB PROTOBUF_CONFIGS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/protobuf/*.cmake")
install(FILES ${PROTOBUF_CONFIGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/protobuf)

file(GLOB OPTIONAL_LITE_CONFIGS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/optional-lite/*.cmake")
install(FILES ${OPTIONAL_LITE_CONFIGS} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/optional-lite)

if (ANDROID)
    file(GLOB NAKAMA_AAR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/libnakama-sdk.aar")
    install(FILES ${NAKAMA_AAR} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
