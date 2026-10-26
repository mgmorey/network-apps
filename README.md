# network-apps

A cross-platform C++23 networking library and accompanying test suite.

## Overview

This repository contains a network library implementation plus test programs. It supports building with both the existing `GNUmakefile` and a new `CMakeLists.txt` for better portability across platforms and toolchains.

Replace `/path/to/network-apps` in the examples with the repository's
location. CMake commands specify source and build directories explicitly, so
they can be run without changing directories.

## Build Options

### GNU Make

The existing build system uses `gmake` and the provided `GNUmakefile`.

```sh
cd network-apps
gmake clean
gmake all
```

### CMake

The CMake build supports the core library, tests, and platform-specific
utilities. You can build in a separate directory:

```sh
cmake -S /path/to/network-apps \
  -B /path/to/network-apps/build-cmake \
  -DCMAKE_BUILD_TYPE=Default
cmake --build /path/to/network-apps/build-cmake
```

CMake supports `Debug`, `Default`, `Minimal`, `Release`, `Small`,
`RelWithDebInfo`, and `MinSizeRel` build types. Compiler selection uses
CMake's standard `CMAKE_CXX_COMPILER` setting. The following options provide
Make-like control over instrumentation and toolchain settings:

```sh
cmake -S /path/to/network-apps \
  -B /path/to/network-apps/build-cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DWITH_COVERAGE=ON \
  -DWITH_SANITIZE=address,undefined \
  -DWITH_LTO=ON \
  -DWITH_LOADMAPS=ON
```

`WITH_SANITIZE` accepts sanitizer names supported by the selected compiler.
Leave it empty to disable sanitizers.
`USING_STDLIB` may be left empty to use the compiler default, or set to
`libc++` or `libstdc++`. For example, select Clang and a specific standard
library or linker with `-DCMAKE_CXX_COMPILER=clang++`,
`-DUSING_STDLIB=libc++`, and `-DUSING_LINKER=lld`. `WITH_FORTIFY_SOURCE`
accepts levels 0 through 3 (or an empty value to disable Fortify); set
`WITH_BACKTRACE=ON` for libstdc++ debug-mode backtraces and
`USING_GDBDWARF` to 2 through 5 to choose the debug information format in
`Default` or `Debug` builds. Coverage is off by default and is available for
`Default` and `Debug` builds. LTO and linker maps are opt-in. Assembler
listings are also opt-in, require GCC with GNU assembler, and are written to
the build directory's `listings/` folder.

On Linux and FreeBSD, the CMake build also creates the `unix-server` and
`unix-client` example programs.

## Installing and Packaging

Install the library, public headers, test executables, and available Unix
examples using CMake's install prefix:

```sh
cmake --install /path/to/network-apps/build-cmake \
  --prefix /path/to/install
```

Create a `.tar.gz` package containing the installed files with:

```sh
cmake --build /path/to/network-apps/build-cmake --target package
```

The archive is written to the build directory.

## Maintenance and Developer Targets

The CMake build provides equivalents for common Make maintenance targets:

```sh
cmake --build /path/to/network-apps/build-cmake --target assembly
cmake --build /path/to/network-apps/build-cmake --target compile-flags
cmake --build /path/to/network-apps/build-cmake --target sizes
cmake --build /path/to/network-apps/build-cmake --target tags
cmake --build /path/to/network-apps/build-cmake --target distclean
```

`assembly` writes per-source `.s` files under `assembly/`; `compile-flags`
writes `compile_flags.txt`; and `sizes` writes built library and executable
sizes to `sizes.txt`, preserving the previous report as `sizes.txt~`. The
`tags` target is available when ctags is installed. Source-count targets are
also available: `count-source-files`,
`count-library-common-source-files`, `count-library-source-files`,
`count-test-source-files`, and `count-unix-source-files`. CMake's built-in
`clean` target removes compiled outputs; `distclean` additionally removes
generated reports and developer artifacts.

## Running Tests

With CMake, the project is integrated with CTest.

- Run all tests:

```sh
cmake --build /path/to/network-apps/build-cmake --target check
```

- Run all tests in parallel:

```sh
cmake --build /path/to/network-apps/build-cmake --target check-parallel
```

## Static Analysis

If `cppcheck` is available when CMake is configured, the `analyze` target is
enabled. The `tidy` target is enabled when `clang-tidy` is available and the
CMake generator exports a compile commands database:

```sh
cmake --build /path/to/network-apps/build-cmake --target analyze
cmake --build /path/to/network-apps/build-cmake --target tidy
```

## Coverage

The CMake build also includes a coverage target.

```sh
cmake --build /path/to/network-apps/build-cmake --target coverage
```

If coverage is enabled, this generates:

- `coverage.json`
- `coverage/coverage.html`

## Notes

- The project is tested on multiple platforms, including Linux, macOS, Cygwin, and MinGW.
- The CMake integration is intended as a portable alternative to the existing `GNUmakefile`.
On Windows, the CMake configuration targets the MinGW/MSYS port and uses its
Windows-specific sources and WinSock library; it does not require Microsoft
Visual Studio.

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
