Nakama Cocos2d-x
=============

> Cocos2d-x 3.17 client for Nakama server 2.x.

Nakama is an open-source distributed server for social and realtime games. For more information have a look at the [server documentation](https://heroiclabs.com/docs/).

If you encounter any issues with the server you can generate diagnostics for us with `nakama doctor`. Send these to support@heroiclabs.com or [open an issue](https://github.com/heroiclabs/nakama/issues).

If you experience any issues with the client, it can be useful to enable debug logs (see [Logging](#Logging) section) and [open an issue](https://github.com/heroiclabs/nakama-cpp/issues).

### Getting Started

#### Prerequisites

To get started using Nakama in Cocos2d-x, you will need the following:

1. [Cocos2d-x Engine](http://www.cocos2d-x.org/download) 3.17 or greater.
2. A compiler for the platform you are developing on, such as [Visual Studio](https://www.visualstudio.com/vs/community/) on Windows or [XCode](https://developer.apple.com/xcode/download/) on OSX.
3. [Nakama C++ SDK](https://github.com/heroiclabs/nakama-cpp/releases)

#### Supported platforms

Windows, Android, iOS, Mac

#### Setup

You'll need to copy the Nakama SDK and cocos2d files you downloaded into the appropriate place. To do this:

1. Open your cocos2d-x project folder in Explorer or Finder.
2. Copy `cocos2d-x` engine folder to `cocos2d` folder
3. Copy the `nakama-cpp-sdk` folder from the Nakama C++ release you downloaded, into your project's folder.

Folder structure should be as following:
```
<Your Project Root>
   cocos2d
   nakama-cpp-sdk
```

4. Integrate the client library into your project:

- add defines:
  - `NAKAMA_API=`
  - `NLOGS_ENABLED`

- add include directory: `nakama-cpp-sdk/include`
- add link directory: `nakama-cpp-sdk/libs/{platform}/{ABI}`
- add link libraries:
  - `nakama-cpp`
  - `grpc++`
  - `libprotobuf`
  - `gpr`
  - `grpc`
  - `cares`
  - `address_sorting`

At this point, you are done. Try to build.

### Usage

Please find in [Nakama C++ README](https://github.com/heroiclabs/nakama-cpp/blob/master/README.md#usage) 

### License

This project is licensed under the [Apache-2 License](https://github.com/heroiclabs/nakama-dotnet/blob/master/LICENSE).

### Special Thanks

Thanks to @dimon4eg for this excellent support on developing Nakama C++, Cocos2d-x and Unreal client libraries.
