# Change Log

All notable changes to this project are documented below.

The format is based on [keep a changelog](http://keepachangelog.com/) and this project uses [semantic versioning](http://semver.org/).

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
