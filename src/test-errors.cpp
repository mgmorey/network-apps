// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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
#include "network/network.hpp"          // Error, RuntimeError,
                                        // get_api_error(),
                                        // get_os_error(),
                                        // reset_api_error(),
                                        // reset_os_error(),
                                        // to_handle(),
                                        // to_name_length(),
                                        // to_os_error(),
                                        // to_path_length(),
                                        // to_sa_length(), to_size(),
                                        // to_sun_length()
#include "network/parse.hpp"            // parse()

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <limits>       // std::numeric_limits
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::Error;
    using Network::LogicError;
    using Network::RuntimeError;
    using Network::ValueError;
    using Network::get_api_error;
    using Network::get_os_error;
    using Network::handle_type;
    using Network::length_type;
    using Network::name_length_max;
    using Network::name_length_min;
    using Network::name_length_type;
    using Network::os_error_type;
    using Network::parse;
#ifndef WIN32
    using Network::path_length_max;
    using Network::path_length_min;
#endif
    using Network::reset_api_error;
    using Network::reset_os_error;
    using Network::name_length_max;
    using Network::name_length_min;
    using Network::name_length_type;
    using Network::os_error_type;
    using Network::sa_length_max;
    using Network::sa_length_min;
#ifndef WIN32
    using Network::sun_length_max;
    using Network::sun_length_min;
#endif
    using Network::to_handle;
    using Network::to_name_length;
    using Network::to_os_error;
#ifndef WIN32
    using Network::to_path_length;
#endif
    using Network::to_sa_length;
    using Network::to_size;
#ifndef WIN32
    using Network::to_sun_length;
#endif

#ifndef WIN32
    constexpr auto expected_handle_re {
        ""
    };
#else
    constexpr auto expected_handle_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of handle_type)"
    };
#endif
    constexpr auto expected_name_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_length_type)"
    };
    constexpr auto expected_os_error_re {
        R"(Value (\d+|-\d+) is out of range \[-?\d+, \d+\] of os_error_type)"
    };
#ifndef WIN32
    constexpr auto expected_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
#endif
    constexpr auto expected_sa_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sa_length_type)"
    };
    constexpr auto expected_std_size_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of std::size_t)"
    };
#ifndef WIN32
    constexpr auto expected_sun_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sun_length_type)"
    };
#endif

    auto is_verbose {false};  // NOLINT

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

    auto test_get_api_error() -> void
    {
        reset_api_error();
        assert(get_api_error() == 0);
    }

    auto test_get_os_error() -> void
    {
        reset_os_error();
        assert(get_os_error() == 0);
    }

    auto test_handle(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_handle(value));
        }
        catch (const ValueError<handle_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_handle() -> void
    {
        test_handle(-1, expected_handle_re);
    }

    auto test_name_length(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_name_length(value));
        }
        catch (const ValueError<name_length_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_name_length() -> void
    {
        test_name_length(name_length_min - 1, expected_name_length_re);
        test_name_length(name_length_max + 1, expected_name_length_re);
    }

    auto test_os_error(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_os_error(value));
        }
        catch (const ValueError<os_error_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_os_error() -> void
    {
        const long long max {std::numeric_limits<os_error_type>::max()};
        const long long min {std::numeric_limits<os_error_type>::min()};
        test_os_error(min - 1, expected_os_error_re);
        test_os_error(max + 1, expected_os_error_re);
    }

#ifndef WIN32

    auto test_path_length(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_path_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_path_length() -> void
    {
        test_path_length(path_length_min - 1, expected_path_length_re);
        test_path_length(path_length_max + 1, expected_path_length_re);
    }

#endif

    auto test_sa_length(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_sa_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_sa_length() -> void
    {
        test_sa_length(sa_length_min - 1, expected_sa_length_re);
        test_sa_length(sa_length_max + 1, expected_sa_length_re);
    }

    auto test_std_size(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_size(value));
        }
        catch (const ValueError<std::size_t>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_std_size() -> void
    {
        test_std_size(-1, expected_std_size_re);
    }

#ifndef WIN32

    auto test_sun_length(auto value, const char* expected_re) -> void
    {
        std::string actual_str;

        try {
            static_cast<void>(to_sun_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_str = error.what();
        }

        const std::regex expected_regex {expected_re};
        assert(std::regex_match(actual_str, expected_regex));
    }

    auto test_sun_length() -> void
    {
        test_sun_length(sun_length_min - 1, expected_sun_length_re);
        test_sun_length(sun_length_max + 1, expected_sun_length_re);
    }

#endif

    auto test_throw_error() -> void
    {
        const std::string expected {"Error"};
        std::string actual;

        try {
            throw Error(expected);
        }
        catch (const Error& error) {
            print(error);
            actual = error.what();
        }

        assert(actual == expected);
    }

    auto test_throw_logic_error() -> void
    {
        const std::string expected {"LogicError"};
        std::string actual;

        try {
            throw LogicError(expected);
        }
        catch (const LogicError& error) {
            print(error);
            actual = error.what();
        }

        assert(actual == expected);
    }

    auto test_throw_runtime_error() -> void
    {
        const std::string expected {"RuntimeError"};
        std::string actual;

        try {
            throw RuntimeError(expected);
        }
        catch (const RuntimeError& error) {
            print(error);
            actual = error.what();
        }

        assert(actual == expected);
    }
}

auto main(int argc, char* argv[]) -> int
{
    try {
        parse_arguments(argc, argv);
        test_get_api_error();
        test_get_os_error();
        test_handle();
        test_name_length();
        test_os_error();
#ifndef WIN32
        test_path_length();
#endif
        test_sa_length();
        test_std_size();
#ifndef WIN32
        test_sun_length();
#endif
        test_throw_error();
        test_throw_logic_error();
        test_throw_runtime_error();
    }
    catch (const Error& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
