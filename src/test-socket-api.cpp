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

#include "network/assert.hpp"           // assert()
#include "network/network.hpp"          // ApiOptions, Error,
                                        // FailMode, Runtime,
                                        // SocketApi, Version, run()
#include "network/parse.hpp"            // parse()
#include "network/quote.hpp"            // quote()
#include "network/socketapi.hpp"        // SocketApi()
#include "network/stop.hpp"             // stop()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <array>        // std::array
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <string_view>  // std::string_view

namespace
{
    using Network::ApiOptions;
    using Network::Error;
    using Network::FailMode;
    using Network::Runtime;
    using Network::SocketApi;
    using Network::Version;
    using Network::quote;
    using Network::parse;

#ifdef WIN32
    constexpr auto expected_code_stopped {WSANOTINITIALISED};
    constexpr auto expected_runtime_platform_re
    {
        "WinSock 2.0"
    };
#else
    constexpr auto expected_code_stopped {0};
    constexpr auto expected_runtime_platform_re {
        "Berkeley Software Distribution Sockets"
    };
#endif
    constexpr auto expected_runtime_version_re {
        "(Version \\d{1,3}\\.\\d{1,3} )?" // NOLINT
    };

    const auto fail_mode {FailMode::return_error};
    auto is_verbose {false};  // NOLINT

    auto get_actual_str(const Runtime& rt) -> std::string
    {
        std::ostringstream oss;
        oss << rt;
        return oss.str();
    }

    auto get_expected_error(Version version) -> std::string_view
    {
        constexpr std::array<std::string_view, 3> errors {
#ifdef WIN32
            "The Windows Sockets version requested is not supported.",
#else
            "",
#endif
            "", "",
        };

        return errors.at(version.major());
    }

    auto get_expected_re() -> std::string
    {
        std::string result;
        result += "(";
        result += expected_runtime_platform_re;
        result += " ";
        result += expected_runtime_version_re;
        result += "Running)";
        return result;
    }

    auto parse_arguments(int argc, char** argv) -> void
    {
        [[maybe_unused]] const auto [args, options] {parse(argc, argv, "v")};

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
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto print(const Runtime& rt) -> void
    {
        if (is_verbose) {
            std::cout << "    Version:\t\t"
                      << rt.version()
                      << std::endl;
            std::cout << "    High Version:\t"
                      << rt.high_version()
                      << std::endl;
            std::cout << "    Description:\t"
                      << quote(rt.description())
                      << std::endl;
            std::cout << "    System Status:\t" << quote(rt.system_status())
                      << std::endl;
        }
        else {
            std::cout << rt << std::endl;
        }
    }

    auto print(ApiOptions ao) -> void
    {
        if (is_verbose) {
            std::cout << "    Input Version:\t";

            if (ao.version()) {
                std::cout << ao.version().value_or(Version {});
            }
            else {
                std::cout << "tNone";
            }

            std::cout << std::endl;
            std::cout << "    Input Fail Mode:\t"
                      << ao.fail_mode()
                      << std::endl;
        }
    }

    auto test(Runtime& rt) -> void
    {
        assert(rt.version() == Version {});
        assert(rt.high_version() == Version {});
        assert(rt.description().empty());
        assert(rt.system_status().empty());
        print(rt);
        assert(!rt.is_running());
        rt.start();
        print(rt);
        assert(rt.is_running());
        const std::string actual_str {get_actual_str(rt)};
        const std::regex expected_regex {get_expected_re()};
        assert(std::regex_match(actual_str, expected_regex));
        const int error_code {rt.stop()};
        assert(!error_code);
        print(rt);
        assert(!rt.is_running());
    }

    auto test(ApiOptions ao, Version version) -> void
    {
        std::cout << "Testing Socket API version: " << version << std::endl;
        print(ao);
        auto sa {SocketApi(ao)};
        test(sa);
    }

    auto test_inactive() -> void
    {
        std::string actual_str;
        int error_code {0};

        try {
            const ApiOptions ao {fail_mode, is_verbose};
            error_code = Network::stop(ao);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str.empty());
        assert(error_code == expected_code_stopped);
    }

    auto test_version(Version version) -> void
    {
        const auto expected_str {get_expected_error(version)};
        std::string actual_str;

        try {
            const ApiOptions ao {version, fail_mode, is_verbose};
            test(ao, version);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str == expected_str);
    }

    auto test_versions() -> void
    {
        const unsigned first {0U};
        const unsigned last {2U};

        for (unsigned major = first; major <= last; ++major) {
            test_version(Version {major});
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        test_versions();
        test_inactive();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
