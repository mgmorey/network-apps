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

#include "network/argumentdata.h"       // ArgumentData
#include "network/argumentspan.h"       // ArgumentSpan, std::span
#include "network/assert.h"             // assert()
#include "network/get-option.h"         // get_optarg(), get_optind(),
                                        // get_option()
#include "network/to-integer.h"         // to_integer()
#include "network/to-size.h"            // to_size()

#include <cstdlib>      // std::abort()
#include <cstring>      // std::strlen()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <string>       // std::string
#include <vector>       // std::vector

namespace TestOption
{
    using Network::ArgumentData;
    using Network::ArgumentSpan;
    using Network::get_optarg;
    using Network::get_optind;
    using Network::get_option;
    using Network::to_integer;
    using Network::to_size;

    auto get_strings(const char* argv0) -> std::vector<std::string>
    {
        return {argv0, "-f", argv0, "-v", "one", "two", "three"};
    }

    auto parse(std::string& filename, bool& verbose,
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
                verbose = true;
                break;
            default:
                std::abort();
            }

        }

        const auto length {to_integer(std::strlen(optstring))};
        assert(get_optind() == optind_begin + length);
    }

    auto print(ArgumentSpan args,
               const std::string& scope = "") -> void
    {
        auto index {0};

        for (const auto& arg : args) {
            std::cout << (scope.empty() ? "Argument" : scope)
                      << ' '
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }
    }

    auto test_arguments(ArgumentSpan args,
                        const char* argv0) -> void
    {
        print(args);
        assert(std::string {args[0]} == argv0);
    }

    auto test_operands(ArgumentSpan args) -> void
    {
        print(args, "Operand");
        assert(std::string {args[0]} == "one");
        assert(std::string {args[1]} == "two");
        assert(std::string {args[2]} == "three");
        assert(args.size() == 3);
    }

    auto test_options(ArgumentSpan args,
                      const char* argv0) -> void
    {
        print(args, "Option");
        assert(std::string {args[0]} == "-f");
        assert(std::string {args[1]} == argv0);
        assert(std::string {args[2]} == "-v");
        assert(args.size() == 3);
    }

    auto test_arguments(int argc, char** argv) -> void
    {
        assert(argc > 0);
        assert(*argv != nullptr);
        ArgumentData data {get_strings(*argv)};
        const std::span args {data.data(), data.size()};
        std::string filename;
        bool verbose {false};
        parse(filename, verbose, args);
        test_arguments(args, *argv);
        auto options {args.subspan(1, to_size(get_optind()) - 1)};
        test_options(options, *argv);
        auto operands {args.subspan(to_size(get_optind()))};
        test_operands(operands);
        assert(filename == *argv);
        assert(verbose);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestOption;

    try {
        test_arguments(argc, argv);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
