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
#include "network/parse.h"              // parse()
#include "network/to-integer.h"         // to_integer()
#include "network/to-name-len.h"        // to_name_len()
#include "network/to-size.h"            // to_size()
#include "network/to-sock-len.h"        // to_sock_len()

#include <cstdlib>      // EXIT_FAILURE, std::exit()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string

namespace TestRanges
{
    using Network::IntegerError;
    using Network::RangeError;
    using Network::SizeError;
    using Network::name_len_max;
    using Network::parse;
    using Network::sock_len_max;
    using Network::to_integer;
    using Network::to_name_len;
    using Network::to_size;
    using Network::to_sock_len;

    static constexpr auto expected_error_integer_re {
        R"(Value (\d+|-\d+) is out of range \[-\d+, \d+\] of int)"
    };
    static constexpr auto expected_error_name_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of name_len_type)"
    };
    static constexpr auto expected_error_sock_len_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of sock_len_type)"
    };
    static constexpr auto expected_error_std_size_re {
        R"(Value (\d+|-\d+) is out of range \[\d+, \d+\] of std::size_t)"
    };

    static bool verbose {false};  // NOLINT

    static auto parse(int argc, char** argv) -> void
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
            verbose = true;
        }

        static_cast<void>(_);
    }

    static auto print(const RangeError& error) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_integer_invalid(std::size_t value) -> void
    {
        std::string actual_error_str;

        try {
            to_integer(value);
        }
        catch (const IntegerError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_integer_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_integer_invalid() -> void
    {
        test_integer_invalid(SIZE_MAX);
    }

    static auto test_name_len_invalid(long value) -> void
    {
        std::string actual_error_str;

        try {
            to_name_len(value);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_name_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_name_len_invalid() -> void
    {
        test_name_len_invalid(-1);
        test_name_len_invalid(name_len_max + 1);
    }

    static auto test_sock_len_invalid(long value) -> void
    {
        std::string actual_error_str;

        try {
            to_sock_len(value);
        }
        catch (const RangeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_sock_len_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_sock_len_invalid() -> void
    {
        test_sock_len_invalid(-1);
        test_sock_len_invalid(sock_len_max + 1);
    }

    static auto test_std_size_invalid(long value) -> void
    {
        std::string actual_error_str;

        try {
            to_size(value);
        }
        catch (const SizeError& error) {
            print(error);
            actual_error_str = error.what();
        }

        const std::regex expected_error_regex {expected_error_std_size_re};
        assert(std::regex_match(actual_error_str, expected_error_regex));
    }

    static auto test_std_size_invalid() -> void
    {
        test_std_size_invalid(-1);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestRanges;

    try {
        parse(argc, argv);
        test_integer_invalid();
        test_name_len_invalid();
        test_sock_len_invalid();
        test_std_size_invalid();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
