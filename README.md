# network-apps

A cross-platform C++23 networking library and accompanying test suite.

## Overview

This repository contains a network library implementation plus test programs. It supports building with both the existing `GNUmakefile` and a new `CMakeLists.txt` for better portability across platforms and toolchains.

The following examples assume the repository has been cloned to a directory named `network-apps`.

## Build Options

### GNU Make

The existing build system uses `gmake` and the provided `GNUmakefile`.

```sh
cd network-apps
gmake clean
gmake all
```

### CMake

A starter `CMakeLists.txt` has been added to support the core library and tests. You can build in a separate directory:

```sh
cd network-apps
mkdir -p build-cmake
cd build-cmake
cmake .. -DCMAKE_BUILD_TYPE=Default
cmake --build .
```

## Running Tests

With CMake, the project is integrated with CTest.

- Run all tests:

```sh
cd network-apps/build-cmake
cmake --build . --target check
```

- Run all tests in parallel:

```sh
cd network-apps/build-cmake
cmake --build . --target check-parallel
```

## Coverage

The CMake build also includes a coverage target.

```sh
cd network-apps/build-cmake
cmake --build . --target coverage
```

If coverage is enabled, this generates:

- `coverage.json`
- `coverage/coverage.html`

## Notes

- The project is tested on multiple platforms, including Linux, macOS, Cygwin, and MinGW.
- The CMake integration is intended as a portable alternative to the existing `GNUmakefile`.

## Platform Support

This project has been tested on the following platforms:

| OS Family | Distro / Environment | OS Version (Port) | GCC | Clang |
|-----------|----------------------|-------------------|-----|-------|
| GNU/Linux | Debian | 13 (x86_64) | 14.2.0 | 19.1.7 |
| GNU/Linux | Fedora | 42 (x86_64) | 15.2.1 | 20.1.8 |
| GNU/Linux | Oracle | 9.6 (x86_64) | 14.2.1 | 19.1.7 |
| GNU/Linux | Oracle | 10.0 (x86_64) | 14.2.1 | 19.1.7 |
| GNU/Linux | openSUSE Tumbleweed | 20250914 (x86_64) | 15.2.0 | 21.1.2 |
| Unix | Apple macOS | 26.0 (arm64) | N/A | 21.1.8 |
| Windows | Cygwin | 10.0 (amd64) | 16.0.0 | 21.1.4 |
| Windows | MinGW | 10.0 (amd64) | 15.1.0 | 21.1.5 |

The various OS ports have been tested on the following processors:

| CPU ISA | CPU Vendor and Microarchitecture | 32 bit | 64 bit |
|---------|----------------------------------|--------|--------|
| ARMv8.4-A | Apple M1 | N/A | arm64 |
| x86-64 | Intel Silvermont, Kaby Lake, and Coffee Lake | N/A | amd64 / x86_64 |
