Nakama Cocos2d-x C++ Client SDK
=============

[![GitHub release](https://img.shields.io/github/release/heroiclabs/nakama-cocos2d-x.svg)](https://github.com/heroiclabs/nakama-cocos2d-x/releases/latest)
[![Forum](https://img.shields.io/badge/forum-online-success.svg)](https://forum.heroiclabs.com)
[![Client guide](https://img.shields.io/badge/client_guide-online-brightgreen)](https://heroiclabs.com/docs/cocos2d-x-client-guide)
[![Reference](https://img.shields.io/badge/reference-online-brightgreen)](https://heroiclabs.github.io/nakama-cpp/html/index.html)
[![License](https://img.shields.io/github/license/heroiclabs/nakama.svg)](https://github.com/heroiclabs/nakama/blob/master/LICENSE)

> Cocos2d-x C++ client for Nakama server.

[Nakama](https://github.com/heroiclabs/nakama) is an open-source server designed to power modern games and apps. Features include user accounts, chat, social, matchmaker, realtime multiplayer, and much [more](https://heroiclabs.com).

This client implements the full API and socket options with the server. It's written in C++ with minimal dependencies to support Cocos2d-x, Unreal and other custom engines and frameworks.

If you experience any issues with the client, it can be useful to enable debug logs (see [Logging](#logging) section) and [open an issue](https://github.com/heroiclabs/nakama-cpp/issues).

Full documentation is online - https://heroiclabs.com/docs

## Notice
**DO NOT** download source code from this repo **if you are not intended to create pull request**. This is not for end users.

## Supported platforms

Nakama Cocos2d-x C++ SDK is released with prebuilt libraries for following platforms and architectures:

- Windows - Visual Studio 2015, 2017, 2019 (x86, x64, Debug, Release)
- Android - Android 4.1 (armeabi-v7a, arm64-v8a, x86, x86_64)
- Linux - Ubuntu 14.04.5 (x86, x64)
- Mac - 10.10+
- iOS - 5.0+ (arm64, armv7, armv7s, x86_64), Bitcode is off

## Getting Started

You'll need to setup the server and database before you can connect with the client. The simplest way is to use Docker but have a look at the [server documentation](https://github.com/heroiclabs/nakama#getting-started) for other options.

1. Install and run the servers. Follow these [instructions](https://heroiclabs.com/docs/install-docker-quickstart).

2. Download the SDK from the [releases page](https://github.com/heroiclabs/nakama-cocos2d-x/releases).

3. Integrate the SDK into your project:

When you've downloaded the SDK archive and extracted it to `NAKAMA_COCOS2D_SDK` folder, you should include it in your project.

Copy cocos2d-x specific files: `NAKAMA_COCOS2D_SDK/NakamaCocos2d` folder to `Classes` folder of your project.

We don't recommend to copy the SDK to your project because it's quite big in size (~1 Gb).

### Setup for Mac and iOS projects

1. Add `NAKAMA_COCOS2D_SDK/include` in `Build Settings > Header Search Paths`
2. Add libs folder in `Build Settings > Library Search Paths`:
    - `NAKAMA_COCOS2D_SDK/libs/ios` - for iOS
    - `NAKAMA_COCOS2D_SDK/libs/mac` - for Mac
3. Add all `.a` files located in libs folder to `General > Frameworks, Libraries, and Embedded Content`
4. Add `Classes/NakamaCocos2d` folder to your XCode project as group (not a reference).

### Setup for Android projects

If you use `CMake` then see [Setup for CMake projects](#setup-for-cmake-projects) section.

If you use `ndk-build` then add following to your `Android.mk` file:

```makefile
# add this to your module
LOCAL_STATIC_LIBRARIES += nakama-cpp

# add this after $(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path, NAKAMA_COCOS2D_SDK)

# add this after $(call import-module, cocos)
$(call import-module, nakama-cpp-android)
```

Android uses a permissions system which determines which platform services the application will request to use and ask permission for from the user. The client uses the network to communicate with the server so you must add the "INTERNET" permission.

```xml
<uses-permission android:name="android.permission.INTERNET"/>
```

### Setup for CMake projects

Open for edit your `CMakeLists.txt` file and find following existing code:
```cmake
# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
```
add next code before:
```cmake
# Cocos2d Nakama sources
list(APPEND GAME_SOURCE
     Classes/NakamaCocos2d/NCocosWebSocket.cpp
     Classes/NakamaCocos2d/NCocosHTTP.cpp
     Classes/NakamaCocos2d/NCocosHelper.cpp
     )
# Cocos2d Nakama headers
list(APPEND GAME_HEADER
     Classes/NakamaCocos2d/NCocosWebSocket.h
     Classes/NakamaCocos2d/NCocosHTTP.h
     Classes/NakamaCocos2d/NCocosLogSink.h
     Classes/NakamaCocos2d/NCocosHelper.h
     )
```

At bottom of your `CMakeLists.txt` file add following:

```cmake
add_subdirectory(NAKAMA_COCOS2D_SDK ${CMAKE_CURRENT_BINARY_DIR}/nakama-cpp)
target_link_libraries(${APP_NAME} ext_nakama-cpp)
```

### Setup for Visual Studio projects

In `Project Settings` add following:

1. Add `NAKAMA_COCOS2D_SDK/include` to `C/C++ > General > Additional Include Directories`
2. Add folder to `Linker > General > Additional Library Directories`:
    - `NAKAMA_COCOS2D_SDK/libs/win32/v140` - for VS 2015 x86
    - `NAKAMA_COCOS2D_SDK/libs/win64/v140` - for VS 2015 x64
    - `NAKAMA_COCOS2D_SDK/libs/win32/v141` - for VS 2017 x86
    - `NAKAMA_COCOS2D_SDK/libs/win64/v141` - for VS 2017 x64
    - `NAKAMA_COCOS2D_SDK/libs/win32/v142` - for VS 2019 x86
    - `NAKAMA_COCOS2D_SDK/libs/win64/v142` - for VS 2019 x64
3. Add all `.lib` files located in above folder to `Linker > Input > Additional Dependencies`
4. Add sources from `Classes/NakamaCocos2d` to your Visual Studio project.

## Threading model

Nakama C++ is designed to use in one thread only.

## Usage

The client object has many methods to execute various features in the server or open realtime socket connections with the server.

Include nakama helper header.

```cpp
#include "NakamaCocos2d/NCocosHelper.h"
```

Initialize logger with debug logging level.

```cpp
using namespace Nakama;

NCocosHelper::init(NLogLevel::Debug);
```

Use the connection credentials to build a client object.

```cpp
NClientParameters parameters;
parameters.serverKey = "defaultkey";
parameters.host = "127.0.0.1";
parameters.port = DEFAULT_PORT;
NClientPtr client = NCocosHelper::createDefaultClient(parameters);
```

The `createDefaultClient` will create HTTP/1.1 client to use REST API.

## Tick

The `tick` method pumps requests queue and executes callbacks in your thread. You must call it periodically (recommended every 50ms) in your thread.

```cpp
auto tickCallback = [this](float dt)
{
    client->tick();
    if (rtClient)
        rtClient->tick();
};

auto scheduler = Director::getInstance()->getScheduler();
scheduler->schedule(tickCallback, this, 0.05f /*sec*/, CC_REPEAT_FOREVER, 0, false, "nakama-tick");
```

Without this the default client and realtime client will not work, and you will not receive responses from the server.

### Authenticate

There's a variety of ways to [authenticate](https://heroiclabs.com/docs/authentication) with the server. Authentication can create a user if they don't already exist with those credentials. It's also easy to authenticate with a social profile from Google Play Games, Facebook, Game Center, etc.

```cpp
string email = "super@heroes.com";
string password = "batsignal";

auto successCallback = [](NSessionPtr session)
{
    CCLOG("session token: %s", session->getAuthToken().c_str());
};

auto errorCallback = [](const NError& error)
{
};

client->authenticateEmail(email, password, "", false, {}, successCallback, errorCallback);
```

### Sessions

When authenticated the server responds with an auth token (JWT) which contains useful properties and gets deserialized into a `NSession` object.

```cpp
CCLOG("%s", session->getAuthToken().c_str()); // raw JWT token
CCLOG("%s", session->getUserId().c_str());
CCLOG("%s", session->getUsername().c_str());
CCLOG("Session has expired: %s", session->isExpired() ? "yes" : "no");
CCLOG("Session expires at: %llu", session->getExpireTime());
CCLOG("Session created at: %llu", session->getCreateTime());
```

It is recommended to store the auth token from the session and check at startup if it has expired. If the token has expired you must reauthenticate. The expiry time of the token can be changed as a setting in the server.

```cpp
string authtoken = "restored from somewhere";
NSessionPtr session = restoreSession(authtoken);
if (session->isExpired()) {
    CCLOG("Session has expired. Must reauthenticate!");
}
```

### Requests

The client includes lots of builtin APIs for various features of the game server. These can be accessed with the async methods. It can also call custom logic as RPC functions on the server. These can also be executed with a socket object.

All requests are sent with a session object which authorizes the client.

```cpp
auto successCallback = [](const NAccount& account)
{
    CCLOG("user id : %s", account.user.id.c_str());
    CCLOG("username: %s", account.user.username.c_str());
    CCLOG("wallet  : %s", account.wallet.c_str());
};

client->getAccount(session, successCallback, errorCallback);
```

### Realtime client

The client can create one or more realtime clients with the server. Each realtime client can have it's own events listener registered for responses received from the server.

```cpp
bool createStatus = true; // if the socket should show the user as online to others.
// define realtime client in your class as NRtClientPtr rtClient;
rtClient = NCocosHelper::createRtClient(client, DEFAULT_PORT);
// define listener in your class as NRtDefaultClientListener listener;
listener.setConnectCallback([]()
{
    CCLOG("Socket connected");
});
rtClient->setListener(&listener);
rtClient->connect(session, createStatus);
```

Don't forget to call `tick` method. See [Tick](#tick) section for details.

### Logging

Client logging is off by default.

To enable logs output to console with debug logging level:

```cpp
NCocosHelper::init(NLogLevel::Debug);
```

## Contribute

The development roadmap is managed as GitHub issues and pull requests are welcome. If you're interested to enhance the code please open an issue to discuss the changes or drop in and discuss it in the [community forum](https://forum.heroiclabs.com).

## Source Builds

The cocos2d-x C++ SDK is based on [General C++ SDK](https://github.com/heroiclabs/nakama-cpp)

### Nakama Cocos2d-x Client guide

You can find Nakama Cocos2d-x Client guide [here](https://heroiclabs.com/docs/cocos2d-x-client-guide/).

## Full Cocos2d-x C++ Client example

You can find the Cocos2d-x C++ Client example [here](https://github.com/heroiclabs/nakama-cocos2d-x/tree/master/example)

## License

This project is licensed under the [Apache-2 License](https://github.com/heroiclabs/nakama-dotnet/blob/master/LICENSE).

## Special Thanks

Thanks to [@dimon4eg](https://github.com/dimon4eg) for this excellent support on developing Nakama C++, Cocos2d-x and Unreal client libraries.
