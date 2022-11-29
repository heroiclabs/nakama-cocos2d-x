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

set(FETCHCONTENT_QUIET OFF)

set(VCPKG_BUILD_TYPE release)
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")
project(nakama-cpp-cocos2d-x LANGUAGES CXX)

 # tell CMake it's okay that we override the cocos options from parent directory
cmake_policy(SET CMP0077 NEW)


set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")
file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

## cocos-2d-x flag
set(BUILD_CPP_TESTS OFF)
set(BUILD_LUA_LIBS OFF)
set(BUILD_CPP_TEMPLATE OFF)
set(BUILD_EDITOR_COCOSTUDIO OFF)
set(BUILD_EDITOR_SPINE OFF)
set(BUILD_EXTENSIONS OFF)

### cocos is not installable so we resort to an add_subdirectory workaround.
add_subdirectory($ENV{VCPKG_ROOT}/buildtrees/cocos2d-x/cocos2d-x-src $ENV{VCPKG_ROOT}/buildtrees/cocos2d-x/cocos2d-x-build-${TARGET_TRIPLET})

add_library(nakama-cpp-cocos2d-x SHARED ${SRCS})

target_include_directories(nakama-cpp-cocos2d-x PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/nakama-cpp-cocos2d-x-config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/nakama-cpp-cocos2d-x-config.cmake"
  INSTALL_DESTINATION share/nakama-cpp-cocos2d-x
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# no ZERO_CHECK target for Xcode
set(CMAKE_SUPPRESS_REGENERATION true)

# bitcode will be deprecated in Xcode 14 which is in beta at the time of this writing
# https://developer.apple.com/documentation/xcode-release-notes/xcode-14-release-notes
set_target_properties(cocos2d external PROPERTIES
    XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
    XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
    XCODE_ATTRIBUTE_CODE_SIGNING_STYLE "AUTOMATIC"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{NAKAMA_EXAMPLE_DEVELOPMENT_TEAM}"
    XCODE_LINK_BUILD_PHASE_MODE KNOWN_LOCATION
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
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

add_subdirectory(example)
