set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)

if (PORT MATCHES "nakama")
    ### bring in Nakama as a shared library.
    set(VCPKG_LIBRARY_LINKAGE dynamic)
else()
    set(VCPKG_LIBRARY_LINKAGE static)
endif()

set(VCPKG_BUILD_TYPE release)
set(CMAKE_GENERATOR Visual Studio 17 2022)

string(APPEND VCPKG_C_FLAGS_RELEASE " /O1 /Ob2 /Gw /Gy")
string(APPEND VCPKG_CXX_FLAGS_RELEASE " /O1 /Ob2 /Gw /Gy")

list(APPEND VCPKG_KEEP_ENV_VARS "PATH" "Path")
