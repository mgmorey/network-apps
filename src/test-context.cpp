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
    using Network::get_hostname;
    using Network::parse;
    using Network::start_context;

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
#else
    constexpr auto expected_code_stopped {0};
    constexpr auto expected_context_platform_re {
        "Berkeley Software Distribution Sockets"
    };
    constexpr auto expected_context_version_re {
        "( Version \\d{1,3}\\.\\d{1,3})?" // NOLINT
    };
#endif
#if defined(OS_CYGWIN_NT)
    constexpr auto expected_error_length {
        ""
    };
#elif defined(OS_DARWIN)
    constexpr auto expected_error_length {
        ""
    };
#elif defined(OS_MINGW64_NT)
    constexpr auto expected_error_length {
        "Call to ::gethostname(, 1) failed with error 10014: "
        "The system detected an invalid pointer address in "
        "attempting to use a pointer argument in a call."
    };
#else
    constexpr auto expected_error_length {
        ""
    };
#endif
#ifdef WIN32
    constexpr auto expected_error_stopped {
        "Call to ::gethostname(, 1024) failed with error 10093: "
        "Either the application has not called WSAStartup, "
        "or WSAStartup failed."
    };
    constexpr auto expected_error_version {
        "The Windows Sockets version requested is not supported."
    };
#else
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

    auto test_hostname_length() -> void;
    auto test_hostname_valid() -> void;

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
        const std::regex expected_regex {get_expected_context_re()};
        assert(std::regex_match(actual_context_str, expected_regex));
        test_hostname_length();
        test_hostname_valid();
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

    auto test_context_stopped() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname());
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_stopped);
    }

    auto test_context_invalid() -> void
    {
        constexpr Version invalid;
        std::string actual_error_str;

        try {
            const auto context {
                start_context(invalid, failure_mode, is_verbose)
            };
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str == expected_error_version);
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
    }

    auto test_hostname_length() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname(1, is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        if (!*expected_error_length) {
            assert(actual_error_str.empty());
        }
        else {
            std::cerr << "expected_error_length = "
                      << expected_error_length
                      << std::endl;
            std::cerr << "actual_error_str = "
                      << actual_error_str
                      << std::endl;
            assert(actual_error_str == expected_error_length);
        }
    }

    auto test_hostname_valid() -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(get_hostname());
        }
        catch (const Error& error) {
            print(error);
            actual_error_str = error.what();
        }

        assert(actual_error_str.empty());
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        test_context_invalid();
        test_context_valid();
        test_context_stopped();
        test_context_inactive();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
