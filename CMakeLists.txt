cmake_minimum_required(VERSION 3.20)

project(network
    VERSION 0.0.1
    DESCRIPTION "A cross-platform C++ network library"
    LANGUAGES CXX
)

# ============================================================================
# Build Configuration
# ============================================================================

# Default to Debug build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Default CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# C++ Standard and Flags
# ============================================================================

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compiler Flags
# ============================================================================

# Common flags
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

# Platform-specific flags
if(APPLE)
    add_compile_options(-fno-common)
endif()

# Build type specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g -O0)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Minimal")
    add_compile_options(-O1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Small")
    add_compile_options(-Os)
elseif(CMAKE_BUILD_TYPE STREQUAL "Default")
    add_compile_options(-g)
endif()

# ============================================================================
# Coverage Support
# ============================================================================

option(WITH_COVERAGE "Enable code coverage instrumentation" ON)

if(WITH_COVERAGE AND (CMAKE_BUILD_TYPE STREQUAL "Default" OR CMAKE_BUILD_TYPE STREQUAL "Debug"))
    add_compile_options(--coverage)
    add_link_options(--coverage)
    add_compile_definitions(CODE_COVERAGE_ENABLED)
endif()

# ============================================================================
# Sanitizers
# ============================================================================

option(WITH_SANITIZE "Enable address sanitizer" ON)

if(WITH_SANITIZE AND CMAKE_BUILD_TYPE STREQUAL "Default")
    add_compile_options(
        -fsanitize=address
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
    )
    add_link_options(-fsanitize=address)
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

# Windows-specific sources (in src/windows/)
set(LIBRARY_WINDOWS_SOURCES
    # Placeholder for future Windows support
    # src/windows/apistate.cpp
    # src/windows/read.cpp
    # src/windows/write.cpp
    # ... etc
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

add_library(network SHARED ${NETWORK_SOURCES})

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
endif()

# ============================================================================
# Create Test Executables
# ============================================================================

enable_testing()
include(CTest)

# Helper function to create test executable
function(add_test_executable test_name test_source)
    add_executable(${test_name} ${test_source})
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
    endif()
    target_link_libraries(${test_name} PRIVATE network)
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

if(WITH_COVERAGE)
    find_program(GCOVR_PATH gcovr REQUIRED)
    
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        COMMAND ${GCOVR_PATH}
            --exclude-unreachable-branches
            --gcov-executable='llvm-cov gcov'
            --json
            --output=coverage.json
        COMMAND ${GCOVR_PATH}
            --exclude-unreachable-branches
            --gcov-executable='llvm-cov gcov'
            --html-details
            --html-theme=github.dark-green
            --html-title='NETWORK Code Coverage Report'
            --json-add-tracefile=coverage.json
            --output=coverage/coverage.html
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS network
        COMMENT "Run tests and generate code coverage report"
    )
endif()

# ============================================================================
# Install Targets
# ============================================================================

install(TARGETS network
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/network
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

# ============================================================================
# Print Configuration Summary
# ============================================================================

message(STATUS "")
message(STATUS "=== ${PROJECT_NAME} Build Configuration ===")
message(STATUS "Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "Coverage:       ${WITH_COVERAGE}")
message(STATUS "Sanitizers:     ${WITH_SANITIZE}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
