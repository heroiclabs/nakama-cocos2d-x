set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_BUILD_TYPE release)

string(APPEND VCPKG_C_FLAGS_RELEASE " /O1 /Ob2 /Gw /Gy")
string(APPEND VCPKG_CXX_FLAGS_RELEASE " /O1 /Ob2 /Gw /Gy")

list(APPEND VCPKG_KEEP_ENV_VARS "PATH" "Path")
