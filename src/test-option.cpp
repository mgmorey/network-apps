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

#include "network/argumentdata.hpp"     // ArgumentData
#include "network/argumentspan.hpp"     // ArgumentSpan, std::span
#include "network/gtest.hpp"
#include "network/error.hpp"            // Error
#include "network/get-option.hpp"       // get_optarg(), get_optind(),
                                        // get_option()
#include "network/to-size.hpp"          // to_size()

#include <cstdlib>      // std::abort()
#include <cstring>      // std::strlen()
#include <iostream>     // std::cerr, std::cout, std::endl
#include <span>         // std::span
#include <string>       // std::string
#include <vector>       // std::vector

namespace
{
    using Network::ArgumentData;
    using Network::ArgumentSpan;
    using Network::Error;
    using Network::get_optarg;
    using Network::get_opterr;
    using Network::get_optind;
    using Network::get_option;
    using Network::get_optopt;
    using Network::to_size;

    auto get_strings(const char* argv0) -> std::vector<std::string>
    {
        return {argv0, "-f", argv0, "-v", "one", "two", "three"};
    }

    auto parse_arguments(std::string& filename, bool& is_verbose,
                         ArgumentSpan args) -> void
    {
        static const char* optstring {"f:v"};
        const auto optind_begin {get_optind()};
        int opt {};

        while ((opt = get_option(args, optstring)) != -1) {
            switch (opt) {
            case 'f':
                filename = get_optarg();
                break;
            case 'v':
                is_verbose = true;
                break;
            default:
                std::abort();
            }

        }

        const auto length {static_cast<int>(std::strlen(optstring))};
        EXPECT_EQ(get_optind(), optind_begin + length);
        static_cast<void>(get_optopt());
    }

    auto print(ArgumentSpan args,
               const std::string& scope = {}) -> void
    {
        auto index {0};

        for (const auto* arg : args) {
            std::cout << (scope.empty() ? "Argument" : scope)
                      << ' '
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }
    }

    auto test_arguments(ArgumentSpan args, const char* argv0) -> void
    {
        print(args);
        ASSERT_FALSE(args.empty());
        EXPECT_EQ(std::string {args.front()}, argv0);
    }

    auto test_operands(ArgumentSpan args) -> void
    {
        print(args, "Operand");
        ASSERT_EQ(args.size(), 3U);
        EXPECT_EQ(std::string {args.front()}, "one");
        EXPECT_EQ(std::string {args.subspan(1).front()}, "two");
        EXPECT_EQ(std::string {args.subspan(2).front()}, "three");
    }

    auto test_options(ArgumentSpan args, const char* argv0) -> void
    {
        print(args, "Option");
        ASSERT_EQ(args.size(), 3U);
        EXPECT_EQ(std::string {args.front()}, "-f");
        EXPECT_EQ(std::string {args.subspan(1).front()}, argv0);
        EXPECT_EQ(std::string {args.subspan(2).front()}, "-v");
    }

    auto test_arguments(const char* argv0) -> void
    {
        ASSERT_NE(argv0, nullptr);
        EXPECT_EQ(get_opterr(), 1);
        ArgumentData data {get_strings(argv0)};
        const std::span args {data.data(), data.size()};
        std::string filename;
        bool is_verbose {false};
        parse_arguments(filename, is_verbose, args);
        test_arguments(args, argv0);
        auto options {args.subspan(1, to_size(get_optind()) - 1)};
        test_options(options, argv0);
        auto operands {args.subspan(to_size(get_optind()))};
        test_operands(operands);
        EXPECT_EQ(filename, argv0);
        EXPECT_TRUE(is_verbose);
    }
}

// NOLINTNEXTLINE(bugprone-throwing-static-initialization)
TEST(Option, ArgumentsOptionsAndOperands)
{
    test_arguments("test-option");
}
