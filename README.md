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

### Visual Studio and MSVC

Install Visual Studio with the **Desktop development with C++** workload,
including an MSVC toolset and Windows SDK. CMake presets provide separate
build directories for Visual Studio 2022 and 2026; use the preset matching
the version installed on the machine. List the available presets with:

```sh
cmake --list-presets
```

For Visual Studio 2022, configure, build, and test with:

```sh
cmake --preset vs2022-x64-debug
cmake --build --preset vs2022-x64-debug --parallel
ctest --preset vs2022-x64-debug
```

For Visual Studio 2026, replace `vs2022-x64-debug` with
`vs2026-x64-debug`. The CMake version must recognize the selected Visual
Studio generator. In VS Code, run **CMake: Select Configure Preset**, choose
the matching Visual Studio preset, then use the CMake Tools build and test
commands. This avoids relying on an implicit compiler or generator selection.

Visual Studio's multi-configuration generator selects the configuration at
build time. The project uses the MSVC standard library and WinSock on Windows.
GCC/Clang-specific options such as coverage, `WITH_SANITIZE`, Fortify Source,
and DWARF selection are not available with MSVC.

The GNU Make build profiles are compiler flag recipes, not CMake
configurations. With a single-configuration generator such as Ninja, CMake's
`CMAKE_BUILD_TYPE` can select the project's `Default`, `Debug`, `Minimal`,
`Release`, or `Small` flags. Visual Studio ignores `CMAKE_BUILD_TYPE` and uses
its configuration selected at build time. The closest MSVC configurations
are approximate and do not reproduce the GNU/Clang flags exactly:

| GNU Make profile | GNU/Clang behavior | Closest Visual Studio configuration |
|------------------|--------------------|-------------------------------------|
| `Default` | Debug symbols, no optimization by default | `Debug` |
| `Debug` | `-Og` and debug symbols | `Debug` |
| `Minimal` | `-O1`, preserve frame pointers | `MinSizeRel` |
| `Release` | `-O2`, keep assertions enabled | `Release` |
| `Small` | `-Os` | `MinSizeRel` |

For example, build the size-oriented `Minimal`/`Small` approximations with
`cmake --build <build-directory> --config MinSizeRel`. MSVC's optimization,
debug-runtime, and assertion behavior can differ; notably, the Make `Release`
profile explicitly keeps assertions enabled. The Visual Studio presets in
`CMakePresets.json` currently define `Debug` only. CMake presets are a
cross-platform format, but these presets use Visual Studio generators and
therefore require Windows and the corresponding Visual Studio installation.
Other platforms can use separate presets with Ninja or another available
generator; compiler-specific flags and supported options still vary by
toolchain.

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

- The project is tested on multiple platforms, including Linux, macOS, Cygwin, MinGW, and Visual Studio.
- The CMake integration is intended as a portable alternative to the existing `GNUmakefile`.
On Windows, CMake supports MinGW/MSYS and Visual Studio with MSVC, using the
Windows-specific sources and WinSock library.

## Platform Support

This project has been tested on the following platforms and toolchains. The
Visual Studio 2026 row records a successful CMake configure and build, with all
12 CTest tests passing.

| OS Family | Distro / Environment | OS Version (Port) | GCC | Clang | MSVC |
|-----------|----------------------|-------------------|-----|-------|-------|
| GNU/Linux | Debian | 13 (x86_64) | 14.2.0 | 19.1.7 | N/A |
| GNU/Linux | Fedora | 42 (x86_64) | 15.2.1 | 20.1.8 | N/A |
| GNU/Linux | Oracle | 9.8 (x86_64) | 14.2.1 | 21.1.8 | N/A |
| GNU/Linux | openSUSE Tumbleweed | 20261024 (x86_64) | 16.2.1 | 23.1.3 | N/A |
| Unix | Apple macOS | 26.0 (arm64) | N/A | 21.1.8 | N/A |
| Windows | Cygwin | 10.0 (amd64) | 16.0.0 | 21.1.8 | N/A |
| Windows | MinGW | 10.0 (amd64) | 15.1.0 | 21.1.5 | N/A |
| Windows | Visual Studio 2026 Professional | Windows (x64) | N/A | N/A | 19.51.36256.0 |

The various OS ports have been tested on the following processors:

| CPU ISA | CPU Vendor and Microarchitecture | 32 bit | 64 bit |
|---------|----------------------------------|--------|--------|
| ARMv8.4-A | Apple M1 | N/A | arm64 |
| x86-64 | Intel Silvermont, Kaby Lake, and Coffee Lake | N/A | amd64 / x86_64 |
