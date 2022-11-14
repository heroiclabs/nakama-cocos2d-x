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

set(FETCHCONTENT_QUIET OFF)

set(VCPKG_BUILD_TYPE "release")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Vcpkg toolchain file")
project(nakama-cpp-cocos2d-x LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(optional-lite)
find_package(nakama-sdk)

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

add_library(nakama-cpp-cocos2d-x SHARED ${SRCS})

target_include_directories(nakama-cpp-cocos2d-x PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(FetchContent)

# uses FETCHCONTENT_UPDATES_DISCONNECTED so that the patch is only applied once.
set(FETCHCONTENT_UPDATES_DISCONNECTED_cocos2d ON)

FetchContent_Declare(
  cocos2d-x
  GIT_REPOSITORY https://github.com/cocos2d/cocos2d-x
  GIT_TAG        90f6542cf7fb081335f04e474b880d7ce8c445a1
  GIT_PROGRESS   TRUE
  USES_TERMINAL_DOWNLOAD TRUE
)

# TODO slim as much as possible, a lot that can be done but cocos must provide options to
# allow us to do it. alternatively we can implement via the patch command but that is a last resort.

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

if(APPLE)
    file(TOUCH ${CMAKE_BINARY_DIR}/dummy.h ${CMAKE_BINARY_DIR}/dummy.cpp)
    set(DUMMY_CPP ${CMAKE_BINARY_DIR}/dummy.cpp)
    set(DUMMY_H ${CMAKE_BINARY_DIR}/dummy.h)

    # Just copy over headers into framework dir, because "official" way of doing it
    # via PUBLIC_HEADER property is full of bugs (see comment on PUBLIC_HEADER property below)
    set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_LIBDIR}/nakama-cpp-cocos2d-x.framework/Headers)

    set_target_properties(nakama-sdk PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A

        # we deliberately dont list all real headers, because it is a can of worms. See:
        # - https://gitlab.kitware.com/cmake/cmake/-/issues/16739
        # - https://gitlab.kitware.com/cmake/cmake/-/issues/22760
        # Probably can be removed when proper FILE_SET support lands: https://gitlab.kitware.com/cmake/cmake/-/issues/23386
        PUBLIC_HEADER "${DUMMY_H}"
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
