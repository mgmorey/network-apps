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
#include "network/charactererror.h"     // CharacterError
#include "network/integererror.h"       // IntegerError
#include "network/namelengtherror.h"    // NameLengthError
#include "network/oserrorerror.h"       // OsErrorError
#ifndef WIN32
#include "network/pathlengtherror.h"    // PathLengthError
#endif
#include "network/rangeerror.h"         // RangeError
#include "network/salengtherror.h"      // SaLengthError
#include "network/sizeerror.h"          // SizeError
#include "network/socketlengtherror.h"  // SocketLengthError
#ifndef WIN32
#include "network/sunlengtherror.h"     // SunLengthError
#endif
#include "network/to-character.h"       // to_character()
#include "network/to-integer.h"         // to_integer()
#include "network/to-name-length.h"     // to_name_length()
#include "network/to-os-error.h"        // to_os_error()
#include "network/to-path-length.h"     // to_path_length()
#include "network/to-sa-len.h"          // to_sa_len()
#include "network/to-size.h"            // to_size()
#include "network/to-socket-length.h"   // to_socket_length()
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
    using Network::OsErrorError;
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
    using Network::name_length_max;
    using Network::name_length_min;
    using Network::os_error_max;
    using Network::os_error_min;
#ifndef WIN32
    using Network::path_length_max;
    using Network::path_length_min;
#endif
    using Network::sa_len_max;
    using Network::sa_len_min;
    using Network::socket_length_max;
    using Network::socket_length_min;
#ifndef WIN32
    using Network::sun_len_max;
    using Network::sun_len_min;
#endif
    using Network::to_character;
    using Network::to_integer;
    using Network::to_name_length;
    using Network::to_os_error;
#ifndef WIN32
    using Network::to_path_length;
#endif
    using Network::to_sa_len;
    using Network::to_size;
    using Network::to_socket_length;
#ifndef WIN32
    using Network::to_sun_len;
#endif

    static constexpr auto expected_error_character_re {
        R"(Value (\d+|-\d+) is out of range \[-?\d+, \d+\] of char)"
    };
    static constexpr auto expected_error_integer_re {
        R"(Value (\d+|-\d+) is out of range \[-\d+, \d+\] of int)"
    };
    static constexpr auto expected_error_name_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_length_type)"
    };
    static constexpr auto expected_error_os_error_re {
        R"(Value (\d+|-\d+) is out of range \[-?\d+, \d+\] of os_error_type)"
    };
#ifndef WIN32
    static constexpr auto expected_error_path_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of path_length_type)"
    };
#endif
    static constexpr auto expected_error_sa_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sa_len_type)"
    };
    static constexpr auto expected_error_socket_length_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of socket_length_type)"
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
        test_integer_invalid(static_cast<long>(INT_MIN) - 1);
        test_integer_invalid(static_cast<long>(INT_MAX) + 1);
    }

    auto test_name_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_name_length(value));
        }
        catch (const NameLengthError& error) {
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
        catch (const OsErrorError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_os_error_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_os_error_invalid() -> void
    {
        test_os_error_invalid(static_cast<long>(os_error_min) - 1);
        test_os_error_invalid(static_cast<long>(os_error_max) + 1);
    }

#ifndef WIN32

    auto test_path_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_path_length(value));
        }
        catch (const PathLengthError& error) {
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

    auto test_socket_length_invalid(auto value) -> void
    {
        std::string actual_error_str;

        try {
            static_cast<void>(to_socket_length(value));
        }
        catch (const SocketLengthError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_socket_length_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    auto test_socket_length_invalid() -> void
    {
        test_socket_length_invalid(socket_length_min - 1);
        test_socket_length_invalid(socket_length_max + 1);
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
        test_name_length_invalid();
        test_os_error_invalid();
#ifndef WIN32
        test_path_length_invalid();
#endif
        test_sa_len_invalid();
        test_socket_length_invalid();
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
