#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.10)

set(APP_NAME nakama-cocos2d-x-example)

project(${APP_NAME})

get_filename_component(NAKAMA_PROJECT "${CMAKE_SOURCE_DIR}" DIRECTORY)

# TODO figure out better way to parameterize this, does cocos cli accept CMake cache variables?
set(NAKAMA_TRIPLET "android-arm64-v8a")

if (${NAKAMA_TRIPLET} STREQUAL "UNDEFINED")
    message(FATAL_ERROR "Set the Nakama triplet to match the directory in 'out' that you'd like to test.")
endif()

set(optional-lite_DIR ${NAKAMA_PROJECT}/out/${NAKAMA_TRIPLET}/share/optional-lite)
set(nakama-sdk_DIR ${NAKAMA_PROJECT}/out/${NAKAMA_TRIPLET}/share/nakama-sdk)

find_package(optional-lite REQUIRED)
find_package(nakama-sdk REQUIRED)

set(COCOS2DX_ROOT_PATH $ENV{COCOS_2DX_ROOT})

### This needs to be one of the first commands in order to prevent code signing issues.
if (${CMAKE_SYSTEM_NAME} STREQUAL "iOS" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # CMake needs to implement a cleaner way of doing this: https://gitlab.kitware.com/cmake/cmake/-/issues/22997 &&
    # https://gitlab.kitware.com/cmake/cmake/-/issues/22240
    set(CMAKE_BUILD_WITH_INSTALL_RPATH YES)
endif()

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)
include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${COCOS2DX_ROOT_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/HelloWorldScene.cpp
     )
list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/HelloWorldScene.h
     )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )

        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()


if(${CMAKE_SYSTEM_NAME} STREQUAL "iOS")
    set_target_properties(${APP_NAME} PROPERTIES
        INSTALL_RPATH @executable_path/Frameworks
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(${APP_NAME} PROPERTIES
        INSTALL_RPATH @executable_path/../Frameworks
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_target_properties(${APP_NAME} PROPERTIES
        INSTALL_RPATH $ORIGIN
    )
endif()

if (APPLE)
    set_target_properties(${APP_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.example"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.heroiclabs.example"
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "YES"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS ${CMAKE_CURRENT_SOURCE_DIR}/example.entitlements
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{NAKAMA_TEST_DEVELOPMENT_TEAM}"
        XCODE_LINK_BUILD_PHASE_MODE KNOWN_LOCATION
        # targets don't work for embedding frameworks, and can't find a better way to get path
        XCODE_EMBED_FRAMEWORKS ${NAKAMA_PROJECT}/out/${NAKAMA_TRIPLET}/lib/nakama-sdk.framework
        XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY		"YES"		# frameworks must be signed by the same developer as the binary
        XCODE_EMBED_FRAMEWORKS_REMOVE_HEADERS_ON_COPY	"YES"
        XCODE_ATTRIBUTE_CODE_SIGN_STYLE Automatic
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/example.plist
    )
endif()


target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist")
    elseif(IOS)
        set_xcode_property(${APP_NAME} INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(LINUX OR WINDOWS)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()

# use Nakama SDK
target_link_libraries(${APP_NAME} nakama-sdk)
