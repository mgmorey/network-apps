# network-apps

A cross-platform C++23 networking library and accompanying test suite.

## Overview

This repository contains a network library implementation plus test programs. It supports building with both the existing `GNUmakefile` and a new `CMakeLists.txt` for better portability across platforms and toolchains.

Run the commands in this README from the repository root unless noted
otherwise.

## Build Options

### GNU Make

The existing build system uses `gmake` and the provided `GNUmakefile`.

```sh
gmake clean
gmake all
```

### CMake

The CMake build supports the core library, tests, and platform-specific
utilities. You can build in a separate directory:

```sh
cmake -S . -B build-cmake -DCMAKE_BUILD_TYPE=Default
cmake --build build-cmake
```

CMake and its presets require version 3.24 or newer.

CMake supports `Debug`, `Default`, `Minimal`, `Release`, `Small`,
`RelWithDebInfo`, and `MinSizeRel` build types. Compiler selection uses
CMake's standard `CMAKE_CXX_COMPILER` setting. The following options provide
Make-like control over instrumentation and toolchain settings:

With multi-configuration GNU/Clang generators, select `Debug`, `Release`,
`RelWithDebInfo`, or `MinSizeRel` at build time with `--config`; the project's
profile-specific flags follow that selection. To use the additional `Default`,
`Minimal`, or `Small` profiles, include them in `CMAKE_CONFIGURATION_TYPES`
when configuring.

Tests and examples are enabled by default. Use the standard CMake
`-DBUILD_TESTING=OFF` option to disable tests or
`-DBUILD_EXAMPLES=OFF` to disable examples:

```sh
cmake -S . -B build-cmake -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF
```

### macOS

On Apple silicon, use the macOS preset to configure for arm64 with Apple
Clang and the Unix Makefiles generator:

```sh
cmake --preset macos-arm64-debug
cmake --build --preset macos-arm64-debug --parallel
ctest --preset macos-arm64-debug
```

The preset writes build files to
`build-cmake/macos-arm64-debug`. Additional ARM64 presets cover the supported
build profiles:

| Preset | Architecture | Build type |
|--------|--------------|------------|
| `macos-arm64-default` | arm64 | Default |
| `macos-arm64-debug` | arm64 | Debug |
| `macos-arm64-minimal` | arm64 | Minimal |
| `macos-arm64-release` | arm64 | Release |
| `macos-arm64-small` | arm64 | Small |

Each preset has matching configure, build, and test preset names.

For local macOS debugging from Emacs, `.dir-locals.el` selects Dape's
`lldb-dap` adapter. Install it with `brew install lldb`; see Dape's [LLDB
adapter documentation](https://github.com/svaante/dape#supported-debug-adapters)
and Homebrew's [`lldb` formula](https://formulae.brew.sh/formula/lldb). Since
Homebrew keeps `lldb` keg-only, add its executable directory to Emacs's `PATH`,
for example in `~/.zprofile`:

```sh
export PATH="$(brew --prefix lldb)/bin:$PATH"
```

Restart Emacs after changing the path. If using GDB directly, macOS may
require code-signing it with the debugger entitlement to access local
processes; see the [GDB Darwin permissions
guide](https://sourceware.org/gdb/wiki/PermissionsDarwin).

### Ubuntu and Podman

The `Containerfile.ubuntu` file in the repository root describes a build image
based on Ubuntu 24.04 with GCC 14 (for C++23 library support), CMake, Ninja,
GDB, `clangd`, Git, and GoogleTest from the distribution package, so
configuration does not download GoogleTest. Build the image with:

```sh
podman build -t network-apps-build:24.04 -f Containerfile.ubuntu .
```

To pull the base image from a registry mirror, add
`--build-arg BASE_IMAGE=<mirror>/library/ubuntu:24.04`. Mount the source tree
into the container and run the Ubuntu presets there. The `--userns=keep-id`
option keeps build output owned by the invoking user, and `:Z` relabels the
mount for SELinux hosts:

```sh
podman run --rm --userns=keep-id \
  -v "$PWD":/src:Z -w /src \
  network-apps-build:24.04 \
  sh -c 'cmake --preset ubuntu-x64-debug && \
         cmake --build --preset ubuntu-x64-debug --parallel && \
         ctest --preset ubuntu-x64-debug'
```

The presets use the Unix Makefiles generator and are enabled on any Linux
host, so they also work outside the container when a suitable toolchain is
installed. Build files are written to `build-cmake/ubuntu-x64-debug`. The
Ubuntu presets mirror the macOS presets, without the architecture setting:

| Preset | Architecture | Build type |
|--------|--------------|------------|
| `ubuntu-x64-default` | x64 | Default |
| `ubuntu-x64-debug` | x64 | Debug |
| `ubuntu-x64-minimal` | x64 | Minimal |
| `ubuntu-x64-release` | x64 | Release |
| `ubuntu-x64-small` | x64 | Small |

Each preset has matching configure, build, and test preset names.

#### Emacs

To edit and debug from Emacs, start a long-lived container and open the source
tree through TRAMP's `podman` method (built into Emacs 29 and later):

```sh
podman run -d --name network-apps-dev --userns=keep-id \
  -v "$PWD":/src:Z -w /src \
  network-apps-build:24.04 sleep infinity
```

Visit `/podman:network-apps-dev:/src/` and run `M-x gdb` there to debug the
binaries in `build-cmake/ubuntu-x64-debug` with the container's GDB. The
`dape` package can debug through GDB's Debug Adapter Protocol support instead:
run `M-x dape`, which defaults to `unix-server`; edit the `:program` value to
debug another binary such as `unix-client`. The `.dir-locals.el` file selects
the macOS ARM64 preset for local macOS Emacs and the Ubuntu x64 preset for
Linux or Podman remote Emacs. It sets `M-x compile` to configure and build with
that preset, selects Dape's `lldb-dap` adapter on local macOS and GDB on Linux
or Podman, and points Eglot at the corresponding `compile_commands.json`.
Emacs ignores directory-local variables on remote files unless
`enable-remote-dir-locals` is non-nil. Set it in the Emacs initialization file
with `(setq enable-remote-dir-locals t)`. Emacs then asks once to trust these
directory-local variables.

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
cmake -S . -B build-cmake \
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
`Default` and `Debug` configurations; with a multi-configuration generator,
build the `coverage` target using `--config Debug` (or `Default` if configured).
LTO and linker maps are opt-in. Assembler
listings are also opt-in, require GCC with GNU assembler, and are written to
the build directory's `listings/` folder.

## Examples

The project builds a Unix-domain client/server example pair on macOS, Linux,
and FreeBSD. It uses sequenced-packet sockets on Linux and FreeBSD, and
newline-delimited stream sockets on macOS:

```sh
cmake --build build-cmake --target examples
```

Start the server in one terminal:

```sh
build-cmake/unix-server
```

Then send a sequence of integers from another terminal; the client prints
their sum:

```sh
build-cmake/unix-client 1 2 3
```

Send `DOWN` to stop the server:

```sh
build-cmake/unix-client DOWN
```

Use `cmake --build build-cmake --target run-examples`
to run the paired example as a smoke test. With GNU Make, use `gmake examples`
to build the programs or `gmake run-examples` to run that smoke test. Examples
are not installed with the library.

Additional examples:

- `resolve-host [hostname]` resolves a host and prints addresses.
- `runtime-info` prints networking runtime version and status.
- `tcp-loopback` binds, connects, and exchanges messages over local TCP.

Build all examples with `cmake --build <build-directory> --target examples`;
run the cross-platform smoke examples with
`cmake --build <build-directory> --target run-examples`.

## Tests

Test targets are enabled by default with `BUILD_TESTING=ON`. CMake first
searches for a system-installed package with `find_package(GTest)`. If none is
available, CMake fetches the pinned GoogleTest 1.17.0 release with
`FetchContent`.

Run the complete CTest suite with:

```sh
cmake --build build-cmake --target check
ctest --test-dir build-cmake --output-on-failure
```

## Installing and Packaging

Install the library and public headers using CMake's install prefix:

```sh
cmake --install build-cmake \
  --prefix install
```

Create a `.tar.gz` package containing the installed files with:

```sh
cmake --build build-cmake --target package
```

The archive is written to the build directory.

## Maintenance and Developer Targets

The CMake build provides equivalents for common Make maintenance targets:

```sh
cmake --build build-cmake --target assembly
cmake --build build-cmake --target compile-flags
cmake --build build-cmake --target sizes
cmake --build build-cmake --target tags
cmake --build build-cmake --target distclean
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
cmake --build build-cmake --target check
```

- Run all tests in parallel:

```sh
cmake --build build-cmake --target check-parallel
```

## Static Analysis

If `cppcheck` is available when CMake is configured, the `analyze` target is
enabled. The `tidy` target is enabled when `clang-tidy` is available and the
CMake generator exports a compile commands database:

```sh
cmake --build build-cmake --target analyze
cmake --build build-cmake --target tidy
```

## Coverage

The CMake build also includes a coverage target.

```sh
cmake --build build-cmake --target coverage
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
12 CTest tests passing. The Ubuntu row records a Podman container build with
the `ubuntu-x64-debug` preset, with all 14 CTest tests passing.

| OS Family | Distro / Environment | OS Version (Port) | GCC | Clang | MSVC |
|-----------|----------------------|-------------------|-----|-------|-------|
| GNU/Linux | Debian | 13 (x86_64) | 14.2.0 | 19.1.7 | N/A |
| GNU/Linux | Fedora | 42 (x86_64) | 15.2.1 | 20.1.8 | N/A |
| GNU/Linux | Oracle | 9.8 (x86_64) | 14.2.1 | 21.1.8 | N/A |
| GNU/Linux | openSUSE Tumbleweed | 20261024 (x86_64) | 16.2.1 | 23.1.3 | N/A |
| GNU/Linux | Ubuntu (Podman container) | 24.04 (x86_64) | 14.2.0 | N/A | N/A |
| Unix | Apple macOS | 26.0 (arm64) | N/A | 21.1.8 | N/A |
| Windows | Cygwin | 10.0 (amd64) | 16.0.0 | 21.1.8 | N/A |
| Windows | MinGW | 10.0 (amd64) | 15.1.0 | 21.1.5 | N/A |
| Windows | Visual Studio 2026 Professional | Windows (x64) | N/A | N/A | 19.51.36256.0 |

The various OS ports have been tested on the following processors:

| CPU ISA | CPU Vendor and Microarchitecture | 32 bit | 64 bit |
|---------|----------------------------------|--------|--------|
| ARMv8.4-A | Apple M1 | N/A | arm64 |
| x86-64 | Intel Silvermont, Kaby Lake, and Coffee Lake | N/A | amd64 / x86_64 |
