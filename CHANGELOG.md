# Change Log

All notable changes to this project are documented below.

The format is based on [keep a changelog](http://keepachangelog.com/) and this project uses [semantic versioning](http://semver.org/).

## [2.6.0] - [2023-02-19]

### Changed
- Updated to Nakama 2.7.1
- Remove buggy Cocos adapters. The new adapters are now native to nakama-cpp.

## [2.5.1] - [2022-01-17]
### Added
- Added realtime parties support.

### Fixed
- Fixed assignment of cursor in listing of storage objects.
- Fixed join group chat by correcting NChannelType enum values.

## [2.4.0] - [2020-11-01]
### Added
- Support server 2.13.0
- Added authentication with Apple ID: `authenticateApple`, `linkApple` and `unlinkApple`
- Added `demoteGroupUsers`
- Added `NClientInterface::rpc` with `http key`
- Added `disableTime` to `NAccount`
- Added `updateTime` to `NFriend`
- Improve future-compatibility with server changes

### Fixed
- Fix listFriends failing on 2.3.0 against 2.13.0 server
- fix crash when response is received after NClient was deleted

## [2.3.0] - [2020-03-01]
### Fixed
- RPC call fails when body is empty.
- RPC payload differences between Rest and RT clients.

### Changed
- Release for Ubuntu 18.04 x64 only (previously was Ubuntu 14.04.5 x86, x64).

## [2.2.1] - 2019-09-28
### Fixed
- REST client: query args are not sent

## [2.2.0] - 2019-09-17
### Added
- Support server 2.7

### Changed
- Following client API has been added/changed to support server 2.7:
  - NSessionInterface
    - getVariables
    - getVariable
  - NClientInterface
    - authenticateDevice
    - authenticateEmail
    - authenticateFacebook
    - authenticateGoogle
    - authenticateGameCenter
    - authenticateCustom
    - authenticateSteam
    - listFriends
    - createGroup
    - listGroupUsers
    - listUserGroups
  - NRtClientInterface
    - joinMatch
- Updated optional lib to v.3.2.0

## [2.1.0] - 2019-06-09
### Added
- REST client (HTTP/1.1).
- Add tournament start active time to the API response.

### Changed
- gRPC client is off.
- Use Nakama as static lib.

## [2.0.3] - 2019-05-05
### Added
- SSL support.
- Client and Realtime Client different host/ssl config.
- Added enums for storage permissions.
- Support Visual Studio 2019.

### Changed
- Use Nakama as shared lib (DLL).
- Updated grpc to 1.19.1.

### Fixed
- Fixed listUserGroups for own user.
- Fixed: success callback is not called for methods with empty response.
- Propagate transport error to realtime client listener.
- Validation of user presences in sendMatchData.
- Xcode warning on required version.

## [2.0.1] - 2019-03-04
### Added
- Initial public release.
- Current limitation - No SSL support
