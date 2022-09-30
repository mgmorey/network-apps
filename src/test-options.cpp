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

#include "network/argument.h"           // Argument
#include "network/argumentdata.h"       // ArgumentData
#include "network/argumentspan.h"       // ArgumentSpan
#include "network/assert.h"             // assert()
#include "network/get-arguments.h"      // get_arguments()
#include "network/get-option.h"         // get_optind()
#include "network/get-options.h"        // get_options()
#include "network/to-integer.h"         // to_integer()
#include "network/to-size.h"            // to_size()

#include <climits>      // SIZE_MAX
#include <cstring>      // std::strlen()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <regex>        // std::regex, std::regex_match
#include <string>       // std::string
#include <vector>       // std::vector

using Network::Argument;
using Network::ArgumentData;
using Network::ArgumentSpan;
using Network::Error;
using Network::IntegerError;
using Network::get_arguments;
using Network::get_optind;
using Network::get_options;
using Network::to_integer;
using Network::to_size;

namespace TestArguments
{
    static constexpr auto expected_error_int_re {
        R"(Value (\d+|-\d+) is out of range \[-\d+, \d+\] of int)"
    };

    static auto get_strings(const char* argv0) -> std::vector<std::string>
    {
        return {argv0, "-f", argv0, "-v", "one", "two", "three"};
    }

    static auto parse(std::string& filename, bool& verbose,
                      ArgumentSpan args) -> void
    {
        static const char* optstring {"f:v"};

        const auto optind_begin {get_optind()};
        auto options {get_options(args, optstring)};
        filename = options['f'];

        if (options.find('v') != options.end()) {
            verbose = true;
        }

        const auto length {to_integer(std::strlen(optstring))};
        assert(get_optind() == optind_begin + length);
    }

    static auto print(const ArgumentSpan args,
                      const std::string& scope = "All") -> void
    {
        auto index {0};

        for (const auto& arg : args) {
            std::cout << scope
                      << " argument "
                      << ++index
                      << ": "
                      << arg
                      << std::endl;
        }
    }

    static auto print(const Error& error, bool verbose) -> void
    {
        if (verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    static auto test_arguments_optional(const ArgumentSpan args,
                                        const char* argv0) -> void
    {
        print(args, "Optional");
        assert(std::string {args[0]} == "-f");
        assert(std::string {args[1]} == argv0);
        assert(std::string {args[2]} == "-v");
        assert(args.size() == 3);
    }

    static auto test_arguments_required(const ArgumentSpan args) ->
        void
    {
        print(args, "Required");
        assert(std::string {args[0]} == "one");
        assert(std::string {args[1]} == "two");
        assert(std::string {args[2]} == "three");
        assert(args.size() == 3);
    }

    static auto test_arguments(const ArgumentSpan args,
                               const char* argv0) -> void
    {
        print(args, "All");
        assert(std::string {args[0]} == argv0);
        assert(std::string {args[1]} == "-f");
        assert(std::string {args[2]} == argv0);
        assert(std::string {args[3]} == "-v");
        assert(std::string {args[4]} == "one");
        assert(std::string {args[5]} == "two");
        assert(std::string {args[6]} == "three");
    }

    static auto test_arguments(int argc, char** argv) -> void
    {
        assert(argc > 0);
        assert(*argv != nullptr);
        ArgumentData data {get_strings(*argv)};
        auto args {get_arguments(data.size(), data.data())};
        std::string filename;
        bool verbose {false};
        parse(filename, verbose, args);
        test_arguments(args, *argv);
        auto optional {args.subspan(1, to_size(get_optind()) - 1)};
        test_arguments_optional(optional, *argv);
        auto required {args.subspan(to_size(get_optind()))};
        test_arguments_required(required);
        assert(filename == *argv);
        assert(verbose);
    }

    static auto test_integer_invalid(std::size_t value, bool verbose) -> void
    {
        std::string actual_error_str;

        try {
            to_integer(value);
        }
        catch (const IntegerError& error) {
            print(error, verbose);
            actual_error_str = error.what();
        }

        const std::regex expected_error_int_regex {expected_error_int_re};
        assert(std::regex_match(actual_error_str, expected_error_int_regex));
    }

    static auto test_integer_invalid(bool verbose) -> void
    {
        test_integer_invalid(SIZE_MAX, verbose);
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestArguments;

    try {
        test_arguments(argc, argv);
        test_integer_invalid(false);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
