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
#include "network/network.hpp"          // Error, FailMode, Runtime,
                                        // RuntimeData, Version,
                                        // WindowsVersion,
                                        // create_runtime(),
                                        // get_hostname(),
                                        // is_running(), run()
#include "network/parse.hpp"            // parse()
#include "network/stop.hpp"             // stop()

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
    using Network::Error;
    using Network::FailMode;
    using Network::Runtime;
    using Network::RuntimeData;
#ifdef WIN32
    using Network::Version;
    using Network::WindowsVersion;
#endif
    using Network::create_runtime;
    using Network::get_hostname;
    using Network::parse;

#ifdef WIN32
    constexpr auto expected_code_stopped {WSANOTINITIALISED};
    constexpr auto expected_error_stopped_re {
        "(Call to ::gethostname\\(.+\\) failed with error \\d+: .+)"
    };
    constexpr auto expected_error_version {
        "The Windows Sockets version requested is not supported."
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
        "Version \\d{1,3}\\.\\d{1,3}" // NOLINT
    };

    const auto fail_mode {FailMode::return_error};
    auto is_verbose {false};  // NOLINT

    auto get_actual_rt_str(const Runtime& rt) -> std::string
    {
        std::ostringstream oss;
        oss << rt;
        return oss.str();
    }

    auto get_expected_runtime_re() -> std::string
    {
        std::string result;
        result += "(";
        result += expected_runtime_platform_re;
        result += " ";
        result += expected_runtime_version_re;
        result += " Running)";
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

    auto print(Runtime& rt, const std::string& description) -> void
    {
        std::cout << "Runtime";

        if (is_verbose) {
            std::cout << ' ' << &rt;
        }

        std::cout << ": " << description << std::endl;

        if (rt.version()) {
            std::cout << "    Version:\t\t"
                      << *rt.version()
                      << std::endl;
        }

        std::cout << "    Description:\t\""
                  << rt.description()
                  << '"'
                  << std::endl;
        std::cout << "    System Status:\t\""
                  << rt.system_status()
                  << '"'
                  << std::endl;
    }

    auto test(Runtime& rt, const std::string& description) -> void
    {
        rt.start();
        print(rt, description + " started");
        assert(is_running(rt));
        const std::string actual_str {get_actual_rt_str(rt)};
        const std::regex expected_regex {get_expected_runtime_re()};
        assert(std::regex_match(actual_str, expected_regex));
        rt.stop();
        print(rt, description + " stopped");
        assert(!rt.error_code());
    }

    auto test(const RuntimeData& rd, const std::string& description) -> void
    {
        auto rt {create_runtime(rd)};
        test(*rt, description);
    }

    auto test_runtime_inactive() -> void
    {
        std::string actual_str;
        int error_code {0};

        try {
            error_code = Network::stop(fail_mode, is_verbose);
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str.empty());
        assert(error_code == expected_code_stopped);
    }

    auto test_runtime_stopped() -> void
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

#ifdef WIN32

    auto test_runtime_invalid() -> void
    {
        std::string actual_str;

        try {
            constexpr Version invalid {0, 0};
            constexpr auto fail_mode_throw {FailMode::throw_error};
            const RuntimeData rd {invalid, fail_mode_throw, is_verbose};
            test_runtime(rd, "invalid");
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        std::cerr << actual_str << std::endl;
        assert(actual_str == expected_error_version);
    }

#endif

    auto test_runtime_valid() -> void
    {
        std::string actual_str;

        try {
#ifdef WIN32
            constexpr Version valid {WindowsVersion::latest};
            const RuntimeData rd {valid, fail_mode, is_verbose};
#else
            const RuntimeData rd {fail_mode, is_verbose};
#endif
            test(rd, "shared");
        }
        catch (const Error& error) {
            print(error);
            actual_str = error.what();
        }

        assert(actual_str.empty());
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
#ifdef WIN32
        test_runtime_invalid();
#endif
        test_runtime_valid();
        test_runtime_stopped();
        test_runtime_inactive();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
