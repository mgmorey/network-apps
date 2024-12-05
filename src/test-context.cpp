// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/assert.h"             // assert()
#include "network/network.h"            // Context, Error,
                                        // OptionalVersion, Version,
                                        // get_hostname()
#include "network/parse.h"              // parse()
#include "network/stop.h"               // stop()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string

namespace
{
    using Network::Context;
    using Network::Error;
    using Network::FailureMode;
    using Network::Hostname;
    using Network::OptionalVersion;
    using Network::Version;
#ifdef WIN32
    using Network::WindowsVersion;
#endif
    using Network::get_hostname;
    using Network::parse;
    using Network::start_context;

    static constexpr Version v0_0 {0, 0};
    static constexpr Version v0_1 {0, 1};
    static constexpr Version v1_0 {1, 0};

    static_assert(v0_0 == Version {} && Version {} == Version {0, 0});
    static_assert(v0_0 != v0_1 && v0_1 != v1_0);
    static_assert(v0_0 < v0_1 && v0_1 < v1_0);
    static_assert(v1_0 > v0_1 && v1_0 > v0_0);

#ifdef WIN32
    static_assert(WORD {WindowsVersion(v0_0)} == 0x0U);        // NOLINT
    static_assert(Version {WindowsVersion(0x0U)} == v0_0);     // NOLINT
    static_assert(WORD {WindowsVersion(v0_1)} == 0x100U);      // NOLINT
    static_assert(Version {WindowsVersion(0x100U)} == v0_1);   // NOLINT
    static_assert(WORD {WindowsVersion(v1_0)} == 0x1U);	      // NOLINT
    static_assert(Version {WindowsVersion(0x1U)} == v1_0);     // NOLINT
#endif

#ifdef WIN32
    constexpr auto expected_code_stopped {WSANOTINITIALISED};
    constexpr auto expected_context_platform_re
    {
        "WinSock 2.0"
    };
    constexpr auto expected_context_version_re
    {
        "( Version \\d{1,3}\\.\\d{1,3})" // NOLINT
    };
    constexpr auto expected_error_stopped {
        "Call to ::gethostname(, 1024) failed with error 10093: "
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    constexpr auto expected_error_version {
        "The Windows Sockets version requested is not supported."
    };
#else
    constexpr auto expected_code_stopped {0};
    constexpr auto expected_context_platform_re {
        "Berkeley Software Distribution Sockets"
    };
    constexpr auto expected_context_version_re {
        "( Version \\d{1,3}\\.\\d{1,3})?" // NOLINT
    };
    constexpr auto expected_error_stopped {""};
    constexpr auto expected_error_version {""};
#endif

    const auto failure_mode {FailureMode::return_error};
    auto is_verbose {false};  // NOLINT

    auto get_expected_context_re() -> std::string
    {
        std::string result;
        result += "(";
        result += expected_context_platform_re;
        result += expected_context_version_re;
        result += " Running)";
        return result;
    }

    auto parse_arguments(int argc, char** argv) -> void
    {
        const auto [_, options] {parse(argc, argv, "v")};

        if (options.contains('?')) {
            std::cerr << "Usage: "
                      << *argv
                      << " [-v]"
                      << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (options.contains('v')) {
            is_verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto print(const Context& context,
               const std::string& description) -> void
    {
        std::cout << "Context";

        if (is_verbose) {
            std::cout << ' '
                      << &context;
        }

        std::cout << ": "
                  << description
                  << std::endl
                  << "    "
                  << context
                  << std::endl;
    }

    auto test_context(const Context& context,
                      const std::string& description,
                      const OptionalVersion& version = {}) -> void
    {
        print(context, description);
        assert(context.is_running());
        assert(!version || context.version() == version);
        std::ostringstream oss;
        oss << context;
        const std::string actual_context_str {oss.str()};
        const std::regex expected_context_regex {get_expected_context_re()};
        assert(std::regex_match(actual_context_str, expected_context_regex));
    }

    auto test_context_inactive() -> void
    {
        int error_code {0};
        std::string actual_error_str;

        try {
            error_code = Network::stop(failure_mode, is_verbose);
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        assert(error_code == expected_code_stopped);
    }

    auto test_context_invalid() -> void
    {
        std::string actual_error_str;

        try {
            const auto context {start_context(v0_0, failure_mode, is_verbose)};
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_version);
        test_context_inactive();
    }

    auto test_context_valid() -> void
    {
        std::string actual_error_str;

        try {
            const auto context_1 {start_context({}, failure_mode, is_verbose)};
            test_context(*context_1, "1", {});
            const auto context_2 {start_context({}, failure_mode, is_verbose)};
            test_context(*context_1, "2", {});
            assert(context_1 != context_2);
            context_1->stop();
            assert(!context_1->error_code());
            context_2->stop();
            assert(!context_2->error_code());
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
        test_context_inactive();
    }

    auto test_no_context() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname(is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_stopped);
        test_context_inactive();
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        test_context_invalid();
        test_context_valid();
        test_no_context();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
