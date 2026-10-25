cmake_minimum_required(VERSION 3.20)

project(network
    VERSION 0.0.1
    DESCRIPTION "A cross-platform C++ network library"
    LANGUAGES CXX
)

# ============================================================================
# Build Configuration
# ============================================================================

# Default to the Make build type if not specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Default CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug Default Minimal Release Small RelWithDebInfo MinSizeRel
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WITH_COVERAGE "Enable code coverage instrumentation" OFF)
set(WITH_SANITIZE "" CACHE STRING "Sanitizers to enable (e.g. address,undefined)")
set_property(CACHE WITH_SANITIZE PROPERTY STRINGS "" address undefined address,undefined)
if(WITH_SANITIZE STREQUAL "ON")
    set(WITH_SANITIZE address CACHE STRING
        "Sanitizers to enable (e.g. address,undefined)" FORCE
    )
elseif(WITH_SANITIZE STREQUAL "OFF")
    set(WITH_SANITIZE "" CACHE STRING
        "Sanitizers to enable (e.g. address,undefined)" FORCE
    )
endif()
set(USING_STDLIB "" CACHE STRING "C++ standard library (libc++ or libstdc++)")
set_property(CACHE USING_STDLIB PROPERTY STRINGS "" libc++ libstdc++)
set(USING_LINKER "" CACHE STRING "Linker to select when using Clang")
set(USING_GDBDWARF "" CACHE STRING "DWARF debug information version (2-5)")
set_property(CACHE USING_GDBDWARF PROPERTY STRINGS "" 2 3 4 5)
set(WITH_FORTIFY_SOURCE "3" CACHE STRING "Fortify source level (0-3)")
set_property(CACHE WITH_FORTIFY_SOURCE PROPERTY STRINGS 0 1 2 3)
option(WITH_BACKTRACE "Enable libstdc++ debug-mode backtraces" OFF)
option(WITH_LTO "Enable link-time optimization" OFF)
option(WITH_LISTINGS "Generate assembler listings when supported" OFF)
option(WITH_LOADMAPS "Generate linker map files" OFF)
if(NOT WITH_FORTIFY_SOURCE STREQUAL "" AND
   NOT WITH_FORTIFY_SOURCE MATCHES "^[0-3]$")
    message(FATAL_ERROR "WITH_FORTIFY_SOURCE must be empty or between 0 and 3")
endif()
if(NOT USING_GDBDWARF STREQUAL "" AND
   NOT USING_GDBDWARF MATCHES "^[2-5]$")
    message(FATAL_ERROR "USING_GDBDWARF must be empty or between 2 and 5")
endif()

# ============================================================================
# C++ Standard and Flags
# ============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compiler Flags
# ============================================================================

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
    message(FATAL_ERROR
        "This project currently requires a GCC- or Clang-compatible compiler"
    )
endif()

include(CheckCXXCompilerFlag)

add_compile_options(-Wall -Werror -Wextra -Wshadow -Wsign-conversion)
if(NOT APPLE)
    add_compile_options(-Wpedantic)
endif()

if(APPLE)
    add_compile_options(-fno-common)
endif()

if(USING_STDLIB STREQUAL "")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(NETWORK_STDLIB libstdc++)
    elseif(APPLE)
        set(NETWORK_STDLIB libc++)
    else()
        set(NETWORK_STDLIB libstdc++)
    endif()
else()
    set(NETWORK_STDLIB ${USING_STDLIB})
endif()

if(NOT NETWORK_STDLIB MATCHES "^(libc\\+\\+|libstdc\\+\\+)$")
    message(FATAL_ERROR
        "USING_STDLIB must be empty, libc++, or libstdc++"
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(NOT USING_STDLIB STREQUAL "")
        add_compile_options(-stdlib=${NETWORK_STDLIB})
        string(REGEX REPLACE "^lib" "" NETWORK_STDLIB_LINK ${NETWORK_STDLIB})
        link_libraries(${NETWORK_STDLIB_LINK})
    endif()
elseif(NOT NETWORK_STDLIB STREQUAL "libstdc++")
    message(FATAL_ERROR "GCC supports libstdc++ only")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(APPLE)
        add_compile_options(-g3 -Og)
    else()
        add_compile_options(-ggdb -Og)
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O2 -UNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2 -g -UNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Os -UNDEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Minimal")
    add_compile_options(-O1 -fno-omit-frame-pointer)
elseif(CMAKE_BUILD_TYPE STREQUAL "Small")
    add_compile_options(-Os)
elseif(CMAKE_BUILD_TYPE STREQUAL "Default")
    add_compile_options(-g)
endif()

if(USING_GDBDWARF AND
   (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Default"))
    set(NETWORK_DWARF_CHECK NETWORK_HAS_DWARF_${USING_GDBDWARF})
    check_cxx_compiler_flag("-gdwarf-${USING_GDBDWARF}"
        ${NETWORK_DWARF_CHECK}
    )
    if(NOT ${NETWORK_DWARF_CHECK})
        message(FATAL_ERROR
            "Compiler does not support DWARF version ${USING_GDBDWARF}"
        )
    endif()
    add_compile_options(-gdwarf-${USING_GDBDWARF})
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-pipe)
endif()

check_cxx_compiler_flag(
    -ftrivial-auto-var-init=pattern
    NETWORK_HAS_TRIVIAL_AUTO_VAR_INIT
)
if(NETWORK_HAS_TRIVIAL_AUTO_VAR_INIT)
    add_compile_options(-ftrivial-auto-var-init=pattern)
endif()

if(NETWORK_STDLIB STREQUAL "libstdc++")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Default")
        add_compile_definitions(_GLIBCXX_DEBUG)
        if(WITH_BACKTRACE)
            add_compile_definitions(_GLIBCXX_DEBUG_BACKTRACE)
            link_libraries(stdc++exp)
        endif()
    elseif(NOT WITH_FORTIFY_SOURCE STREQUAL "")
        add_compile_options(
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=${WITH_FORTIFY_SOURCE}
        )
    endif()
elseif(NOT WITH_FORTIFY_SOURCE STREQUAL "")
    add_compile_options(
        -U_FORTIFY_SOURCE
        -D_FORTIFY_SOURCE=${WITH_FORTIFY_SOURCE}
    )
endif()

if(USING_LINKER)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_link_options(-fuse-ld=${USING_LINKER})
    else()
        message(FATAL_ERROR "USING_LINKER is supported only with Clang")
    endif()
endif()

if(WITH_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT NETWORK_IPO_SUPPORTED OUTPUT NETWORK_IPO_ERROR)
    if(NOT NETWORK_IPO_SUPPORTED)
        message(FATAL_ERROR "LTO is not supported: ${NETWORK_IPO_ERROR}")
    endif()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(WITH_BACKTRACE)
    if(NOT NETWORK_STDLIB STREQUAL "libstdc++")
        message(FATAL_ERROR "WITH_BACKTRACE requires libstdc++")
    elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND
           NOT CMAKE_BUILD_TYPE STREQUAL "Default")
        message(FATAL_ERROR
            "WITH_BACKTRACE requires a Debug or Default build"
        )
    endif()
endif()

# ============================================================================
# Coverage Support
# ============================================================================

if(WITH_COVERAGE AND (CMAKE_BUILD_TYPE STREQUAL "Default" OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_options(--coverage)
    add_link_options(--coverage)
    add_compile_definitions(CODE_COVERAGE_ENABLED)
endif()

# ============================================================================
# Sanitizers
# ============================================================================

if(WITH_SANITIZE)
    include(CheckCXXSourceCompiles)
    string(MAKE_C_IDENTIFIER "${WITH_SANITIZE}" NETWORK_SANITIZER_ID)
    set(NETWORK_SANITIZER_CHECK
        NETWORK_HAS_SANITIZER_${NETWORK_SANITIZER_ID}
    )
    set(NETWORK_SAVED_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    set(NETWORK_SAVED_REQUIRED_LINK_OPTIONS "${CMAKE_REQUIRED_LINK_OPTIONS}")
    string(APPEND CMAKE_REQUIRED_FLAGS " -fsanitize=${WITH_SANITIZE}")
    list(APPEND CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=${WITH_SANITIZE}")
    check_cxx_source_compiles(
        "int main() { return 0; }"
        ${NETWORK_SANITIZER_CHECK}
    )
    set(CMAKE_REQUIRED_FLAGS "${NETWORK_SAVED_REQUIRED_FLAGS}")
    set(CMAKE_REQUIRED_LINK_OPTIONS "${NETWORK_SAVED_REQUIRED_LINK_OPTIONS}")
    if(NOT ${NETWORK_SANITIZER_CHECK})
        message(FATAL_ERROR
            "Compiler does not support sanitizer: ${WITH_SANITIZE}"
        )
    endif()
    add_compile_options(
        -fsanitize=${WITH_SANITIZE}
        -fno-omit-frame-pointer
    )
    add_link_options(-fsanitize=${WITH_SANITIZE})
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR
       CMAKE_BUILD_TYPE STREQUAL "Minimal" OR
       CMAKE_BUILD_TYPE STREQUAL "Small")
        add_compile_options(-fno-optimize-sibling-calls)
    endif()
endif()

# ============================================================================
# Source Files
# ============================================================================

# Common library sources
set(LIBRARY_COMMON_SOURCES
    src/accept-socket.cpp
    src/accept-socketcore.cpp
    src/acceptdata.cpp
    src/address-sa.cpp
    src/address-sin.cpp
    src/address-sin6.cpp
    src/address.cpp
    src/addresserror.cpp
    src/addresslist.cpp
    src/argumentdata.cpp
    src/binarybuffer.cpp
    src/bind-endpoint.cpp
    src/close.cpp
    src/connect-endpoint.cpp
    src/create-runtime.cpp
    src/create-socket-acceptdata.cpp
    src/create-socket-handle.cpp
    src/create-socket-hints.cpp
    src/create-socketresult.cpp
    src/error.cpp
    src/familyerror.cpp
    src/format.cpp
    src/get-endpoint.cpp
    src/get-endpointresult.cpp
    src/get-hostname-charspan.cpp
    src/get-hostname-runtime.cpp
    src/get-hostnameresult.cpp
    src/get-name.cpp
    src/get-namehandler.cpp
    src/get-nameresult.cpp
    src/get-openhandler.cpp
    src/get-operands.cpp
    src/get-option.cpp
    src/get-options.cpp
    src/get-runtime.cpp
    src/get-sa-family.cpp
    src/get-sa-length.cpp
    src/get-sa-pointer.cpp
    src/get-sa-span.cpp
    src/get-sin-addr.cpp
    src/get-sin-pointer.cpp
    src/get-sin-port.cpp
    src/get-sin6-addr.cpp
    src/get-sin6-pointer.cpp
    src/get-sin6-port.cpp
    src/inetsocket.cpp
    src/listen.cpp
    src/logicerror.cpp
    src/open-endpoint.cpp
    src/open-handle.cpp
    src/openinputs.cpp
    src/oserror.cpp
    src/parse-argumentspan.cpp
    src/parse.cpp
    src/quote.cpp
    src/rangeerror.cpp
    src/reset-api-error.cpp
    src/reset-os-error.cpp
    src/run.cpp
    src/runtimeerror.cpp
    src/shutdown.cpp
    src/socketapi.cpp
    src/socketcore.cpp
    src/socketdata.cpp
    src/socketfamily.cpp
    src/socketflags.cpp
    src/sockethost.cpp
    src/socketlimits.cpp
    src/socketprotocol.cpp
    src/sockettemplate.cpp
    src/sockettype.cpp
    src/stream-address.cpp
    src/stream-addrinfo.cpp
    src/stream-socket.cpp
    src/stream-version.cpp
    src/textbuffer.cpp
    src/to-bytestring-void.cpp
    src/to-string-bytespan.cpp
    src/to-string-in-addr.cpp
    src/to-string-in6-addr.cpp
    src/to-string-runtime.cpp
    src/to-string-void.cpp
    src/validate-bs.cpp
    src/validate-sa.cpp
    src/validate-sin.cpp
    src/validate-sin6.cpp
)

# Platform-specific native sources (required for all platforms)
# These are the "native" sources that platforms provide implementations for
set(LIBRARY_NATIVE_SOURCES
    # These only exist in platform-specific directories
    # They will be provided by LIBRARY_UNIX_SOURCES or LIBRARY_WINDOWS_SOURCES
)

# Unix-specific sources (in src/unix/)
# Includes both native implementations and Unix-specific functionality
set(LIBRARY_UNIX_SOURCES
    src/unix/address-sun.cpp
    src/unix/apistate.cpp
    src/unix/bind-path.cpp
    src/unix/connect-path.cpp
    src/unix/create-socket-socketdata.cpp
    src/unix/create-socketpair.cpp
    src/unix/create-socketpairresult.cpp
    src/unix/format-ai-error.cpp
    src/unix/format-os-error.cpp
    src/unix/get-api-error.cpp
    src/unix/get-os-error.cpp
    src/unix/get-path-length.cpp
    src/unix/get-path-pointer.cpp
    src/unix/get-sun-length.cpp
    src/unix/get-sun-pointer.cpp
    src/unix/read.cpp
    src/unix/set-api-error.cpp
    src/unix/set-os-error.cpp
    src/unix/start.cpp
    src/unix/stop.cpp
    src/unix/to-bytestring-path.cpp
    src/unix/to-path.cpp
    src/unix/unixsocket.cpp
    src/unix/validate-path.cpp
    src/unix/validate-sun.cpp
    src/unix/write.cpp
)

# Windows-specific native sources (in src/windows/)
set(LIBRARY_WINDOWS_SOURCES
    src/windows/apistate.cpp
    src/windows/create-socket-socketdata.cpp
    src/windows/format-ai-error.cpp
    src/windows/format-os-error.cpp
    src/windows/get-api-error.cpp
    src/windows/get-os-error.cpp
    src/windows/read.cpp
    src/windows/set-api-error.cpp
    src/windows/set-os-error.cpp
    src/windows/start.cpp
    src/windows/stop.cpp
    src/windows/write.cpp
)

# Test sources (common)
set(TEST_COMMON_SOURCES
    src/test-address.cpp
    src/test-bind.cpp
    src/test-connect.cpp
    src/test-errors.cpp
    src/test-host.cpp
    src/test-hostname.cpp
    src/test-option.cpp
    src/test-parse.cpp
    src/test-runtime.cpp
    src/test-socket-api.cpp
    src/test-socket-data.cpp
    src/test-socket-inet.cpp
)

# Test sources (Unix-specific)
set(TEST_UNIX_SOURCES
    src/unix/test-socket-pair.cpp
    src/unix/test-socket-unix.cpp
)

# Unix example programs are enabled on the same platforms as the Make build.
set(UNIX_PROGRAM_SOURCES)
if(CMAKE_SYSTEM_NAME MATCHES "^(Linux|FreeBSD)$")
    set(UNIX_PROGRAM_SOURCES
        src/unix/unix-client.cpp
        src/unix/unix-server.cpp
    )
endif()

# ============================================================================
# Build the Shared Library
# ============================================================================

# Start with common sources
set(NETWORK_SOURCES ${LIBRARY_COMMON_SOURCES})

# Add platform-specific sources
if(UNIX)
    list(APPEND NETWORK_SOURCES ${LIBRARY_UNIX_SOURCES})
elseif(WIN32)
    list(APPEND NETWORK_SOURCES ${LIBRARY_WINDOWS_SOURCES})
endif()

function(network_enable_link_map target_name)
    if(WITH_LOADMAPS)
        if(APPLE)
            target_link_options(${target_name} PRIVATE
                "LINKER:-map,${PROJECT_BINARY_DIR}/${target_name}.map"
            )
        else()
            target_link_options(${target_name} PRIVATE
                "LINKER:-Map,${PROJECT_BINARY_DIR}/${target_name}.map"
            )
        endif()
    endif()
endfunction()

if(WITH_LISTINGS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT APPLE)
        file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/listings)
        set(LISTING_SOURCES
            ${NETWORK_SOURCES}
            ${TEST_COMMON_SOURCES}
            ${UNIX_PROGRAM_SOURCES}
        )
        if(UNIX)
            list(APPEND LISTING_SOURCES ${TEST_UNIX_SOURCES})
        endif()
        foreach(source IN LISTS LISTING_SOURCES)
            get_filename_component(source_name ${source} NAME_WE)
            set_source_files_properties(${source} PROPERTIES
                COMPILE_OPTIONS
                    "-Wa,-adghln=${PROJECT_BINARY_DIR}/listings/${source_name}.lst"
            )
        endforeach()
    else()
        message(STATUS
            "Assembler listings require GCC with GNU assembler; listings are disabled"
        )
    endif()
endif()

add_library(network SHARED ${NETWORK_SOURCES})
network_enable_link_map(network)

# Set library properties
set_target_properties(network PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)

# Set include directories for the library
target_include_directories(network PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

if(UNIX)
    target_include_directories(network PRIVATE
        ${PROJECT_SOURCE_DIR}/include/unix
    )
elseif(WIN32)
    target_include_directories(network PRIVATE
        ${PROJECT_SOURCE_DIR}/include/windows
    )
    target_link_libraries(network PRIVATE ws2_32)
endif()

if(APPLE)
    set(NETWORK_INSTALL_RPATH "@loader_path/../lib")
elseif(UNIX)
    set(NETWORK_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# ============================================================================
# Create Test Executables
# ============================================================================

enable_testing()
include(CTest)

# Helper function to create test executable
set(TEST_EXECUTABLES)

function(add_test_executable test_name test_source)
    add_executable(${test_name} ${test_source})
    set(TEST_EXECUTABLES ${TEST_EXECUTABLES} ${test_name} PARENT_SCOPE)
    target_include_directories(${test_name} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
    if(UNIX)
        target_include_directories(${test_name} PRIVATE
            ${PROJECT_SOURCE_DIR}/include/unix
        )
    elseif(WIN32)
        target_include_directories(${test_name} PRIVATE
            ${PROJECT_SOURCE_DIR}/include/windows
        )
        target_link_libraries(${test_name} PRIVATE ws2_32)
    endif()
    target_link_libraries(${test_name} PRIVATE network)
    network_enable_link_map(${test_name})
    if(UNIX)
        set_target_properties(${test_name} PROPERTIES
            INSTALL_RPATH "${NETWORK_INSTALL_RPATH}"
        )
    endif()
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES LABELS "unit")
endfunction()

# Common tests
add_test_executable(test-address src/test-address.cpp)
add_test_executable(test-bind src/test-bind.cpp)
add_test_executable(test-connect src/test-connect.cpp)
add_test_executable(test-errors src/test-errors.cpp)
add_test_executable(test-host src/test-host.cpp)
add_test_executable(test-hostname src/test-hostname.cpp)
add_test_executable(test-option src/test-option.cpp)
add_test_executable(test-parse src/test-parse.cpp)
add_test_executable(test-runtime src/test-runtime.cpp)
add_test_executable(test-socket-api src/test-socket-api.cpp)
add_test_executable(test-socket-data src/test-socket-data.cpp)
add_test_executable(test-socket-inet src/test-socket-inet.cpp)

# Unix-specific tests
if(UNIX)
    add_test_executable(test-socket-pair src/unix/test-socket-pair.cpp)
    set_tests_properties(test-socket-pair PROPERTIES LABELS "unit;unix")
    add_test_executable(test-socket-unix src/unix/test-socket-unix.cpp)
    set_tests_properties(test-socket-unix PROPERTIES LABELS "unit;unix")
endif()

foreach(program_source IN LISTS UNIX_PROGRAM_SOURCES)
    get_filename_component(program_name ${program_source} NAME_WE)
    add_executable(${program_name} ${program_source})
    target_include_directories(${program_name} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/unix
    )
    target_link_libraries(${program_name} PRIVATE network)
    network_enable_link_map(${program_name})
    set_target_properties(${program_name} PROPERTIES
        INSTALL_RPATH "${NETWORK_INSTALL_RPATH}"
    )
endforeach()

# ============================================================================
# Maintenance and Developer Targets
# ============================================================================

set(PROJECT_SOURCES ${NETWORK_SOURCES} ${TEST_COMMON_SOURCES})
if(UNIX)
    list(APPEND PROJECT_SOURCES ${TEST_UNIX_SOURCES})
endif()
list(APPEND PROJECT_SOURCES ${UNIX_PROGRAM_SOURCES})

set(LIBRARY_SOURCES ${NETWORK_SOURCES})
set(TEST_SOURCES ${TEST_COMMON_SOURCES})
if(UNIX)
    list(APPEND TEST_SOURCES ${TEST_UNIX_SOURCES})
endif()
set(UNIX_SOURCES ${UNIX_PROGRAM_SOURCES})

function(add_source_count_target target_name)
    set(source_list ${ARGN})
    list(LENGTH source_list source_count)
    add_custom_target(${target_name}
        COMMAND ${CMAKE_COMMAND} -E echo ${source_count}
        VERBATIM
    )
endfunction()

add_source_count_target(count-library-common-source-files
    ${LIBRARY_COMMON_SOURCES}
)
add_source_count_target(count-library-source-files ${LIBRARY_SOURCES})
add_source_count_target(count-source-files ${PROJECT_SOURCES})
add_source_count_target(count-test-source-files ${TEST_SOURCES})
add_source_count_target(count-unix-source-files ${UNIX_SOURCES})

get_directory_property(NETWORK_COMPILE_OPTIONS COMPILE_OPTIONS)
get_directory_property(NETWORK_COMPILE_DEFINITIONS COMPILE_DEFINITIONS)
get_target_property(NETWORK_INCLUDE_DIRECTORIES network INCLUDE_DIRECTORIES)
string(TOUPPER "${CMAKE_BUILD_TYPE}" NETWORK_BUILD_TYPE_UPPER)
set(NETWORK_CONFIG_FLAGS
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${NETWORK_BUILD_TYPE_UPPER}}"
)
separate_arguments(NETWORK_CONFIG_FLAGS_LIST NATIVE_COMMAND
    "${NETWORK_CONFIG_FLAGS} ${CMAKE_CXX_COMPILER_ARG1}"
)
set(NETWORK_COMPILE_FLAGS ${NETWORK_CONFIG_FLAGS_LIST} ${NETWORK_COMPILE_OPTIONS})
if(UNIX)
    list(APPEND NETWORK_COMPILE_FLAGS -fPIC)
endif()
if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    foreach(architecture IN LISTS CMAKE_OSX_ARCHITECTURES)
        list(APPEND NETWORK_COMPILE_FLAGS -arch ${architecture})
    endforeach()
endif()
if(APPLE AND CMAKE_OSX_SYSROOT)
    list(APPEND NETWORK_COMPILE_FLAGS -isysroot ${CMAKE_OSX_SYSROOT})
endif()
foreach(definition IN LISTS NETWORK_COMPILE_DEFINITIONS)
    list(APPEND NETWORK_COMPILE_FLAGS "-D${definition}")
endforeach()
foreach(include_directory IN LISTS NETWORK_INCLUDE_DIRECTORIES)
    list(APPEND NETWORK_COMPILE_FLAGS "-I${include_directory}")
endforeach()
list(APPEND NETWORK_COMPILE_FLAGS "-std=c++${CMAKE_CXX_STANDARD}")
list(JOIN NETWORK_COMPILE_FLAGS " " NETWORK_COMPILE_FLAGS_TEXT)
file(GENERATE
    OUTPUT "${PROJECT_BINARY_DIR}/compile_flags.txt"
    CONTENT "${CMAKE_CXX_COMPILER} ${NETWORK_COMPILE_FLAGS_TEXT}\n"
)
add_custom_target(compile-flags
    DEPENDS "${PROJECT_BINARY_DIR}/compile_flags.txt"
)

set(ASSEMBLY_OUTPUTS)
foreach(source IN LISTS PROJECT_SOURCES)
    string(REPLACE "/" "_" assembly_name "${source}")
    set(assembly_output "${PROJECT_BINARY_DIR}/assembly/${assembly_name}.s")
    add_custom_command(
        OUTPUT "${assembly_output}"
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${PROJECT_BINARY_DIR}/assembly"
        COMMAND ${CMAKE_CXX_COMPILER}
            ${NETWORK_COMPILE_FLAGS}
            -S "${PROJECT_SOURCE_DIR}/${source}"
            -o "${assembly_output}"
        DEPENDS "${PROJECT_SOURCE_DIR}/${source}"
        VERBATIM
    )
    list(APPEND ASSEMBLY_OUTPUTS "${assembly_output}")
endforeach()
add_custom_target(assembly DEPENDS ${ASSEMBLY_OUTPUTS})

find_program(CTAGS_PATH NAMES ctags universal-ctags)
if(CTAGS_PATH)
    set(TAG_INPUTS
        ${PROJECT_SOURCE_DIR}/include/network
        ${PROJECT_SOURCE_DIR}/src
    )
    if(UNIX)
        list(APPEND TAG_INPUTS
            ${PROJECT_SOURCE_DIR}/include/unix/network
            ${PROJECT_SOURCE_DIR}/src/unix
        )
    elseif(WIN32)
        list(APPEND TAG_INPUTS ${PROJECT_SOURCE_DIR}/include/windows/network)
    endif()
    add_custom_target(tags
        COMMAND ${CTAGS_PATH}
            --output-format=etags
            -f ${PROJECT_BINARY_DIR}/TAGS
            ${TAG_INPUTS}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generate Emacs TAGS"
        VERBATIM
    )
else()
    message(STATUS "ctags not found; tags target is disabled")
endif()

find_program(SIZE_PATH NAMES llvm-size size)
if(SIZE_PATH)
    set(SIZE_TARGETS network ${TEST_EXECUTABLES})
    if(UNIX_PROGRAM_SOURCES)
        list(APPEND SIZE_TARGETS unix-client unix-server)
    endif()
    set(SIZE_COMMAND_ARGS)
    foreach(target_name IN LISTS SIZE_TARGETS)
        list(APPEND SIZE_COMMAND_ARGS $<TARGET_FILE:${target_name}>)
    endforeach()
    add_custom_target(sizes
        COMMAND ${CMAKE_COMMAND}
            "-DSIZE_TOOL=${SIZE_PATH}"
            "-DSIZE_FILES=${SIZE_COMMAND_ARGS}"
            "-DREPORT_PATH=${PROJECT_BINARY_DIR}/sizes.txt"
            -P "${PROJECT_SOURCE_DIR}/cmake/ReportSizes.cmake"
        DEPENDS ${SIZE_TARGETS}
        COMMENT "Write executable and library sizes to sizes.txt"
        VERBATIM
    )
else()
    message(STATUS "size utility not found; sizes target is disabled")
endif()

# Convenience target for running all tests
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Run all CTest tests"
)

# Convenience target for running tests in parallel
add_custom_target(check-parallel
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j 4
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Run all CTest tests in parallel"
)

# ============================================================================
# Coverage Target (Optional)
# ============================================================================

if(WITH_COVERAGE AND
   (CMAKE_BUILD_TYPE STREQUAL "Default" OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
    find_program(GCOVR_PATH gcovr REQUIRED)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        find_program(GCOV_PATH
            NAMES
                gcov-${CMAKE_CXX_COMPILER_VERSION}
                gcov
            REQUIRED
        )
        set(GCOVR_GCOV_EXECUTABLE ${GCOV_PATH})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        find_program(LLVM_COV_PATH
            NAMES llvm-cov
            REQUIRED
        )
        set(GCOVR_GCOV_EXECUTABLE "${LLVM_COV_PATH} gcov")
    else()
        message(FATAL_ERROR
            "Coverage is not configured for ${CMAKE_CXX_COMPILER_ID}"
        )
    endif()

    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${GCOVR_PATH}
            --root=${PROJECT_SOURCE_DIR}
            --object-directory=${PROJECT_BINARY_DIR}
            --exclude-unreachable-branches
            --gcov-executable=${GCOVR_GCOV_EXECUTABLE}
            --json
            --output=coverage.json
            ${PROJECT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory coverage
        COMMAND ${GCOVR_PATH}
            --root=${PROJECT_SOURCE_DIR}
            --object-directory=${PROJECT_BINARY_DIR}
            --exclude-unreachable-branches
            --gcov-executable=${GCOVR_GCOV_EXECUTABLE}
            --html-details
            --html-theme=github.dark-green
            --html-title='NETWORK Code Coverage Report'
            --json-add-tracefile=coverage.json
            --output=coverage/coverage.html
            ${PROJECT_BINARY_DIR}
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${TEST_EXECUTABLES}
        COMMENT "Run tests and generate code coverage report"
    )
elseif(WITH_COVERAGE)
    message(STATUS "Coverage is supported only for Default and Debug builds")
endif()

# ============================================================================
# Static Analysis Targets (Optional)
# ============================================================================

set(ANALYSIS_SOURCES ${NETWORK_SOURCES} ${TEST_COMMON_SOURCES})
if(UNIX)
    list(APPEND ANALYSIS_SOURCES ${TEST_UNIX_SOURCES})
    set(PLATFORM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/unix)
elseif(WIN32)
    set(PLATFORM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/windows)
endif()
list(APPEND ANALYSIS_SOURCES ${UNIX_PROGRAM_SOURCES})

find_program(CPPCHECK_PATH cppcheck)
if(CPPCHECK_PATH)
    add_custom_target(analyze
        COMMAND ${CMAKE_COMMAND} -E make_directory
            ${PROJECT_BINARY_DIR}/cppcheck
        COMMAND ${CPPCHECK_PATH}
            --check-level=exhaustive
            --checkers-report=${PROJECT_BINARY_DIR}/cppcheck.log
            --cppcheck-build-dir=${PROJECT_BINARY_DIR}/cppcheck
            --enable=all
            --inline-suppr
            --quiet
            --std=c++23
            --suppress=missingIncludeSystem
            -I${PROJECT_SOURCE_DIR}/include
            -I${PLATFORM_INCLUDE_DIR}
            ${ANALYSIS_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Run cppcheck"
        VERBATIM
    )
else()
    message(STATUS "cppcheck not found; analyze target is disabled")
endif()

find_program(CLANG_TIDY_PATH NAMES clang-tidy)
if(CLANG_TIDY_PATH AND CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
    set(CLANG_TIDY_EXTRA_ARGS)
    if(APPLE)
        find_program(XCRUN_PATH xcrun)
        if(XCRUN_PATH)
            execute_process(
                COMMAND ${XCRUN_PATH} --show-sdk-path
                OUTPUT_VARIABLE APPLE_SDK_PATH
                OUTPUT_STRIP_TRAILING_WHITESPACE
                COMMAND_ERROR_IS_FATAL ANY
            )
            list(APPEND CLANG_TIDY_EXTRA_ARGS
                --extra-arg-before=-isysroot
                --extra-arg-before=${APPLE_SDK_PATH}
            )
        endif()
    endif()

    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_PATH}
            --config-file=${PROJECT_SOURCE_DIR}/.clang-tidy
            --header-filter=.*
            ${CLANG_TIDY_EXTRA_ARGS}
            -p ${PROJECT_BINARY_DIR}
            ${ANALYSIS_SOURCES}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Run clang-tidy"
        VERBATIM
    )
elseif(CLANG_TIDY_PATH)
    message(STATUS
        "clang-tidy requires a compile commands database; tidy target is disabled for this generator"
    )
else()
    message(STATUS "clang-tidy not found; tidy target is disabled")
endif()

# ============================================================================
# Install Targets
# ============================================================================

install(TARGETS network
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Remove extra generated artifacts not covered by the generator's clean target.
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf
        "${PROJECT_BINARY_DIR}/assembly"
        "${PROJECT_BINARY_DIR}/cppcheck"
        "${PROJECT_BINARY_DIR}/coverage"
    COMMAND ${CMAKE_COMMAND} -E rm -f
        "${PROJECT_BINARY_DIR}/TAGS"
        "${PROJECT_BINARY_DIR}/compile_flags.txt"
        "${PROJECT_BINARY_DIR}/coverage.json"
        "${PROJECT_BINARY_DIR}/cppcheck.log"
        "${PROJECT_BINARY_DIR}/network.map"
        "${PROJECT_BINARY_DIR}/sizes.txt"
        "${PROJECT_BINARY_DIR}/sizes.txt~"
        "${PROJECT_BINARY_DIR}/network-${PROJECT_VERSION}.tar.gz"
    COMMENT "Remove generated developer artifacts"
)

install(TARGETS ${TEST_EXECUTABLES}
    RUNTIME DESTINATION bin
)

if(UNIX_PROGRAM_SOURCES)
    install(TARGETS unix-client unix-server
        RUNTIME DESTINATION bin
    )
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/network
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

if(WIN32)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/windows/network
        DESTINATION include/windows
        FILES_MATCHING PATTERN "*.hpp"
    )
elseif(UNIX)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/unix/network
        DESTINATION include/unix
        FILES_MATCHING PATTERN "*.hpp"
    )
endif()

# ============================================================================
# Package
# ============================================================================

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_GENERATOR TGZ)
include(CPack)

# ============================================================================
# Print Configuration Summary
# ============================================================================

message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Build Configuration ===")
message(STATUS "Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Library:    ${NETWORK_STDLIB}")
message(STATUS "Coverage:       ${WITH_COVERAGE}")
message(STATUS "Sanitizers:     ${WITH_SANITIZE}")
message(STATUS "LTO:            ${WITH_LTO}")
message(STATUS "Linker:         ${USING_LINKER}")
message(STATUS "DWARF:          ${USING_GDBDWARF}")
message(STATUS "Fortify:        ${WITH_FORTIFY_SOURCE}")
message(STATUS "Backtraces:     ${WITH_BACKTRACE}")
message(STATUS "Listings:       ${WITH_LISTINGS}")
message(STATUS "Linker maps:    ${WITH_LOADMAPS}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
