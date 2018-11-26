Nakama Cocos2d-x
=============

> Cocos2d-x 3.16 client for Nakama server.

Nakama is an open-source distributed server for social and realtime games. For more information have a look at the [server documentation](https://heroiclabs.com/docs/).

If you encounter any issues with the server you can generate diagnostics for us with `nakama doctor`. Send these to support@heroiclabs.com or [open an issue](https://github.com/heroiclabs/nakama/issues). If you experience any issues with the client, it can be useful to enable trace (`.Trace(true)`) to produce logs and [open an issue](https://github.com/heroiclabs/nakama-cocos2d-x/issues).

### Getting Started

#### Prerequisites

To get started using Nakama in Cocos2d-x, you will need the following:

1. [Cocos2d-x Engine](http://www.cocos2d-x.org/download) 3.16 or greater.
1. A compiler for the platform you are developing on, such as [Visual Studio](https://www.visualstudio.com/vs/community/) on Windows or [XCode](https://developer.apple.com/xcode/download/) on OSX.
1. [nakama-cocos2d-x](https://github.com/heroiclabs/nakama-cocos2d-x/releases)

#### Setup

To use nakama-cocos2d-x in your cocos2d-x project, you'll need to copy the nakama-cocos2d-x files you downloaded into the appropriate place. To do this:

1. Open your cocos2d-x project folder in Explorer or Finder.
2. Copy cocos2d-x engine folder to cocos2d folder
3. Copy the Nakama and ThirdParty folders from the nakama-cocos2d-x release you downloaded, into Classes folder.

Folder structure should be as following:
<Your Project Root>
   cocos2d
   Classes
       Nakama
       ThirdParty

4. Now add all .cpp, .cc files from Nakama folder to your project:
    a) Windows - proj.win32\<Project Name>.sln
    b) Android - proj.android\jni\Android.mk
    c) iOS/Mac - proj.ios_mac\NakamaCocos2d.xcodeproj

5. Add link libraries to your project:
    libprotobuf - from Classes\ThirdParty\lib\<platform>
    websockets  - cocos2d\external\websockets\prebuilt\<platform>

6. Add defines for project:
    NAKAMA_API=
    COCOS2D

7. Add include directories for project:
    Classes\Nakama\Public\NakamaSDK
    Classes\Nakama\Public
    Classes\Nakama\Private
    Classes\ThirdParty\include

At this point, you are done. Try to build.

#### Server connect

To create a client and connect to the Nakama server with the default settings:

```c++
#include "NClient.h"

using namespace Nakama;

NClient client = NClient::Default("defaultkey");
```
