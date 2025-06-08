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
#include "network/network.hpp"          // ApiInput, Error, FailMode,
                                        // Runtime, Version,
                                        // create_runtime(),
                                        // get_hostname(),
                                        // is_running(), run()
#include "network/parse.hpp"            // parse()
#include "network/quote.hpp"            // quote()
#include "network/stop.hpp"             // stop()

#ifdef WIN32
#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#endif

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <map>          // std::map
#include <regex>        // std::regex, std::regex_match
#include <sstream>      // std::ostringstream
#include <string>       // std::string

namespace
{
    using Network::ApiInput;
    using Network::Error;
    using Network::FailMode;
    using Network::Runtime;
    using Network::Version;
    using Network::create_runtime;
    using Network::get_hostname;
    using Network::quote;
    using Network::parse;

#ifdef WIN32
    constexpr auto expected_code_stopped {WSANOTINITIALISED};
    constexpr auto expected_error_stopped_re {
        "(Call to ::gethostname\\(.+\\) failed with error \\d+: .+)"
    };
    constexpr auto expected_runtime_platform_re
    {
        "WinSock 2.0"
    };
#else
    constexpr auto expected_code_stopped {0};
    constexpr auto expected_error_stopped_re {""};
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

    auto get_expected_errors() -> std::map<unsigned short, std::string>
    {
        std::map<unsigned short, std::string> errors;
#ifdef WIN32
        errors[0] =
            "The Windows Sockets version requested is not supported.";
#else
        errors[0] = "";
#endif
        errors[1] = "";
        errors[2] = "";
        return errors;
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
        std::cout << "    Version:\t\t"
                  << rt.version()
                  << std::endl;
        std::cout << "    High Version:\t"
                  << rt.high_version()
                  << std::endl;
        std::cout << "    Description:\t"
                  << quote(rt.description())
                  << std::endl;
        std::cout << "    System Status:\t"
                  << quote(rt.system_status())
                  << std::endl;
        std::cout << "    Error Code:\t\t"
                  << rt.error_code()
                  << std::endl;
        std::cout << "    Is Started:\t\t"
                  << rt.is_started()
                  << std::endl;
    }

    auto print(const ApiInput& rd) -> void
    {

        if (rd.version()) {
            std::cout << "    Input Version:\t"
                      << rd.version().value_or(Version {})
                      << std::endl;
        }
        else {
            std::cout << "    Input Version:\tNone"
                      << std::endl;
        }

        std::cout << "    Input Fail Mode:\t"
                  << rd.fail_mode()
                  << std::endl;
    }

    auto test(Runtime& rt) -> void
    {
        assert(rt.version() == Version {});
        assert(rt.high_version() == Version {});
        assert(rt.description() == std::string_view {});
        assert(rt.system_status() == std::string_view {});
        assert(!rt.error_code());
        assert(!rt.is_started());
        print(rt);
        assert(!is_running(rt));
        rt.start();
        assert(!rt.error_code());
        print(rt);
        assert(is_running(rt));
        const std::string actual_str {get_actual_str(rt)};
        const std::regex expected_regex {get_expected_re()};
        assert(std::regex_match(actual_str, expected_regex));
        rt.stop();
        assert(!rt.error_code());
        print(rt);
        assert(!is_running(rt));
    }

    auto test(const ApiInput& rd, Version version) -> void
    {
        std::cout << "Testing Socket API version: " << version << std::endl;
        print(rd);
        auto rt {create_runtime(rd)};
        test(*rt);
    }

    auto test_inactive() -> void
    {
        std::string actual_str;
        int error_code {0};

        try {
            const ApiInput ai {fail_mode, is_verbose};
            error_code = Network::stop(ai);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str.empty());
        assert(error_code == expected_code_stopped);
    }

    auto test_stopped() -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(get_hostname(is_verbose));
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_error_stopped_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_version(unsigned short major,
                      const std::string& expected_str) -> void
    {
        std::string actual_str;

        try {
            const Version version {major, 0};
            const ApiInput ai {version, fail_mode, is_verbose};
            test(ai, version);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str == expected_str);
    }

    auto test_versions() -> void
    {
        const std::map<unsigned short, std::string> errors
        {
            get_expected_errors()
        };

        for (unsigned short major = 0; major < 3; ++major) {
            test_version(major, errors.at(major));
        }
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        test_versions();
        test_stopped();
        test_inactive();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
