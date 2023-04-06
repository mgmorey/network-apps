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
#include "network/to-character.h"       // to_character()
#include "network/to-integer.h"         // to_integer()
#include "network/to-name-len.h"        // to_name_len()
#include "network/to-path-len.h"        // to_path_len()
#include "network/to-sa-len.h"          // to_sa_len()
#include "network/to-size.h"            // to_size()
#include "network/to-sock-len.h"        // to_sock_len()
#include "network/to-sun-len.h"         // to_sun_len()

#include <climits>      // INT_MAX, INT_MIN
#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace TestRanges
{
    using Network::CharacterError;
    using Network::IntegerError;
    using Network::NameLengthError;
#ifndef WIN32
    using Network::PathLengthError;
#endif
    using Network::RangeError;
    using Network::SaLengthError;
    using Network::SizeError;
    using Network::SocketLengthError;
#ifndef WIN32
    using Network::SunLengthError;
#endif
    using Network::name_len_max;
    using Network::name_len_min;
#ifndef WIN32
    using Network::path_len_max;
    using Network::path_len_min;
#endif
    using Network::sa_len_max;
    using Network::sa_len_min;
    using Network::sock_len_max;
    using Network::sock_len_min;
#ifndef WIN32
    using Network::sun_len_max;
    using Network::sun_len_min;
#endif
    using Network::to_character;
    using Network::to_integer;
    using Network::to_name_len;
#ifndef WIN32
    using Network::to_path_len;
#endif
    using Network::to_sa_len;
    using Network::to_size;
    using Network::to_sock_len;
#ifndef WIN32
    using Network::to_sun_len;
#endif

    static constexpr auto expected_error_character_re {
        R"(Value (\d+|-\d+) is out of range \[-?\d+, \d+\] of char)"
    };
    static constexpr auto expected_error_integer_re {
        R"(Value (\d+|-\d+) is out of range \[-\d+, \d+\] of int)"
    };
    static constexpr auto expected_error_name_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_len_type)"
    };
#ifndef WIN32
    static constexpr auto expected_error_path_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_len_type)"
    };
#endif
    static constexpr auto expected_error_sa_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sa_len_type)"
    };
    static constexpr auto expected_error_sock_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sock_len_type)"
    };
    static constexpr auto expected_error_std_size_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of std::size_t)"
    };
#ifndef WIN32
    static constexpr auto expected_error_sun_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sun_len_type)"
    };
#endif

    auto print(const RangeError& error) -> void
    {
        std::cout << "Exception: "
                  << error.what()
                  << std::endl;
    }

    auto test_character_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_character(value));
        }
        catch (const CharacterError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_character_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_character_invalid() -> void
    {
        test_character_invalid(CHAR_MIN - 1);
        test_character_invalid(CHAR_MAX + 1);
    }

    auto test_integer_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_integer(value));
        }
        catch (const IntegerError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_integer_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_integer_invalid() -> void
    {
        test_integer_invalid(static_cast<long long>(INT_MIN) - 1);
        test_integer_invalid(static_cast<long long>(INT_MAX) + 1);
    }

    auto test_name_len_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_name_len(value));
        }
        catch (const NameLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_name_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_name_len_invalid() -> void
    {
        test_name_len_invalid(name_len_min - 1);
        test_name_len_invalid(name_len_max + 1);
    }

#ifndef WIN32

    auto test_path_len_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_path_len(value));
        }
        catch (const PathLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_path_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_path_len_invalid() -> void
    {
        test_path_len_invalid(path_len_min - 1);
        test_path_len_invalid(path_len_max + 1);
    }

#endif

    auto test_sa_len_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_sa_len(value));
        }
        catch (const SaLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sa_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sa_len_invalid() -> void
    {
        test_sa_len_invalid(sa_len_min - 1);
        test_sa_len_invalid(sa_len_max + 1);
    }

    auto test_sock_len_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_sock_len(value));
        }
        catch (const SocketLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sock_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sock_len_invalid() -> void
    {
        test_sock_len_invalid(sock_len_min - 1);
        test_sock_len_invalid(sock_len_max + 1);
    }

    auto test_std_size_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_size(value));
        }
        catch (const SizeError& error) {
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

    auto test_sun_len_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_sun_len(value));
        }
        catch (const SunLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sun_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_sun_len_invalid() -> void
    {
        test_sun_len_invalid(sun_len_min - 1);
        test_sun_len_invalid(sun_len_max + 1);
    }

#endif
}

auto main() -> int
{
    using namespace TestRanges;

    try {
        test_character_invalid();
        test_integer_invalid();
        test_name_len_invalid();
#ifndef WIN32
        test_path_len_invalid();
#endif
        test_sa_len_invalid();
        test_sock_len_invalid();
        test_std_size_invalid();
#ifndef WIN32
        test_sun_len_invalid();
#endif
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
