## Release Instructions

### New releases

Release for Cocos2d-x should be made after the main release (https://github.com/heroiclabs/nakama-cpp).

Download and extract Nakama C++ SDK from https://github.com/heroiclabs/nakama-cpp/releases

Copy all (except `nakama-cpp-android` and `CMakeLists.txt`) to `nakama-cocos2d-x-sdk`

Remove `zlib` libs as cocos2d has own zlib.

Release folder structure:
```
nakama-cocos2d-x-sdk/
├── include/
│   ├── nakama-cpp/
│   └── nonstd/
├── libs/
├── nakama-cpp-android/
├── NakamaCocos2d/
├── CMakeLists.txt
├── LICENSE
├── CHANGELOG.md
└── README.md
```

### Full release workflow

The development team use these steps to build and upload a release.

1. Make archive from `nakama-cocos2d-x-sdk` folder and name it `nakama-cocos2d-x-sdk_x.x.x.zip`

2. Login and create a [new draft release](https://github.com/heroiclabs/nakama-cocos2d-x/releases/new) on GitHub.

3. Repeat the changelog in the release description.

4. Attach `nakama-cocos2d-x-sdk_x.x.x.zip`

5. Publish the release.
