Nakama Cocos2d-x Client Example
=============

> Nakama Cocos2d-x client example for Nakama server 2.x.

Nakama is an open-source distributed server for social and realtime games. For more information have a look at the [server documentation](https://heroiclabs.com/docs/).

If you encounter any issues with the server you can generate diagnostics for us with `nakama doctor`. Send these to support@heroiclabs.com or [open an issue](https://github.com/heroiclabs/nakama/issues).

### Getting Started

#### Prerequisites

To build example, you will need the following:

1. [Cocos2d-x Engine](http://www.cocos2d-x.org/download) 3.17 or greater.
2. [Nakama Cocos2d-x C++ SDK](https://github.com/heroiclabs/nakama-cocos2d-x/releases)
3. A compiler for the platform you are developing on, such as [Visual Studio](https://www.visualstudio.com/vs/community/) on Windows or [XCode](https://developer.apple.com/xcode/download/) on OSX.

#### Supported platforms

Windows, Android, iOS, Mac, Linux

#### Setup

You'll need to copy the Nakama C++ SDK and cocos2d files you downloaded into the appropriate place. To do this:

1. Copy `cocos2d-x` engine folder to `example/cocos2d`
2. Copy the `nakama-cocos2d-x-sdk` folder from the Nakama Cocos2d C++ release you downloaded, to `example/nakama-cocos2d-x-sdk`.

Folder structure should be as following:
```
example/
   cocos2d/
   nakama-cocos2d-x-sdk/
```

### Nakama Cocos2d-x Client guide

You can find Nakama Cocos2d-x Client guide [here](https://heroiclabs.com/docs/cocos2d-x-client-guide/).

### License

This project is licensed under the [Apache-2 License](https://github.com/heroiclabs/nakama-dotnet/blob/master/LICENSE).

### Special Thanks

Thanks to [@dimon4eg](https://github.com/dimon4eg) for this excellent support on developing Nakama C++, Cocos2d-x and Unreal client libraries.
