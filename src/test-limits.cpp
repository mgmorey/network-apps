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
#include "network/network.hpp"          // RangeError, ValueError,
                                        // to_name_length(),
                                        // to_os_error(),
                                        // to_path_length(),
                                        // to_sa_length(), to_size(),
                                        // to_sun_length()

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <limits>       // std::numeric_limits
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace
{
    using Network::RangeError;
    using Network::ValueError;
    using Network::length_type;
    using Network::name_length_max;
    using Network::name_length_min;
    using Network::name_length_type;
    using Network::os_error_type;
#ifndef WIN32
    using Network::path_length_max;
    using Network::path_length_min;
#endif
    using Network::sa_length_max;
    using Network::sa_length_min;
#ifndef WIN32
    using Network::sun_length_max;
    using Network::sun_length_min;
#endif
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

    constexpr auto expected_error_name_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_length_type)"
    };
    constexpr auto expected_error_os_error_re {
        R"(Value (\d+|-\d+) is out of range \[-?\d+, \d+\] of os_error_type)"
    };
#ifndef WIN32
    constexpr auto expected_error_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
#endif
    constexpr auto expected_error_sa_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sa_length_type)"
    };
    constexpr auto expected_error_std_size_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of std::size_t)"
    };
#ifndef WIN32
    constexpr auto expected_error_sun_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sun_length_type)"
    };
#endif

    auto print(const RangeError& error) -> void
    {
        std::cout << "Exception: "
                  << error.what()
                  << std::endl;
    }

    auto test_name_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_name_length(value));
        }
        catch (const ValueError<name_length_type>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_name_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_name_length_invalid() -> void
    {
        test_name_length_invalid(name_length_min - 1);
        test_name_length_invalid(name_length_max + 1);
    }

    auto test_os_error_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_os_error(value));
        }
        catch (const ValueError<os_error_type>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_os_error_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_os_error_invalid() -> void
    {
        const long long max {std::numeric_limits<os_error_type>::max()};
        const long long min {std::numeric_limits<os_error_type>::min()};
        test_os_error_invalid(min - 1);
        test_os_error_invalid(max + 1);
    }

#ifndef WIN32

    auto test_path_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_path_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_path_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_path_length_invalid() -> void
    {
        test_path_length_invalid(path_length_min - 1);
        test_path_length_invalid(path_length_max + 1);
    }

#endif

    auto test_sa_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_sa_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sa_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sa_length_invalid() -> void
    {
        test_sa_length_invalid(sa_length_min - 1);
        test_sa_length_invalid(sa_length_max + 1);
    }

    auto test_std_size_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_size(value));
        }
        catch (const ValueError<std::size_t>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_std_size_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_std_size_invalid() -> void
    {
        test_std_size_invalid(-1);
    }

#ifndef WIN32

    auto test_sun_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_sun_length(value));
        }
        catch (const ValueError<length_type>& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sun_length_invalid() -> void
    {
        test_sun_length_invalid(sun_length_min - 1);
        test_sun_length_invalid(sun_length_max + 1);
    }

#endif
}

auto main() -> int
{
    try {
        test_name_length_invalid();
        test_os_error_invalid();
#ifndef WIN32
        test_path_length_invalid();
#endif
        test_sa_length_invalid();
        test_std_size_invalid();
#ifndef WIN32
        test_sun_length_invalid();
#endif
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
