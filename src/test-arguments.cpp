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

#include "network/arguments.h"          // Arguments
#include "network/assert.h"             // assert()
#include "network/get-option.h"         // get_optarg(), get_optind(),
                                        // get_option()
#include "network/to-integer.h"         // to_integer()

#include <algorithm>    // std::transform()
#include <cstdlib>      // EXIT_FAILURE, std::exit(), std::free()
#include <cstring>      // strdup(), std::strlen()
#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl
#include <iterator>     // std::back_inserter()
#include <string>       // std::string
#include <vector>       // std::vector

using Network::Arguments;
using Network::get_optarg;
using Network::get_optind;
using Network::get_option;
using Network::to_integer;

namespace TestArguments
{
    using ArgumentVector = std::vector<Arguments::Argument>;
    using StringVector = std::vector<std::string>;

    class ArgumentData
    {
    public:
        explicit ArgumentData(const StringVector& strings)
        {
            std::transform(strings.begin(), strings.end(),
                           std::back_inserter(m_data),
                           [&](const std::string& datum) {
                               return ::strdup(datum.c_str());
                           });
        }

        ~ArgumentData()
        {
            for (auto* arg : m_data) {
                ::free(arg);  // NOLINT
            }
        }

        auto data() -> Arguments::Argument*
        {
            return m_data.data();
        }

        [[nodiscard]] auto size() const -> std::size_t
        {
            return m_data.size();
        }

    private:
        ArgumentVector m_data;
    };

    static auto get_strings(const char* argv0) -> StringVector
    {
        return {
            argv0,
            "-f",
            argv0,
            "-v",
            "one",
            "two",
            "three"
        };
    }

    static auto parse(std::string& filename, bool& verbose,
                      const Arguments& args) -> void
    {
        static const char* opts {"f:v"};

        auto optind_begin {get_optind()};
        int opt {};

        while ((opt = get_option(args, opts)) != -1) {
            switch (opt) {
            case 'f':
                filename = get_optarg();
                break;
            case 'v':
                verbose = true;
                break;
            default:
                abort();
            }

        }

        const int length = to_integer(std::strlen(opts));
        assert(get_optind() == optind_begin + length);
    }

    static auto print(const Arguments::ArgumentSpan& args,
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

    static auto test_optional(const Arguments::ArgumentSpan& args,
                              const char* argv0) -> void
    {
        print(args, "Optional");
        assert(std::string {args[0]} == "-f");
        assert(std::string {args[1]} == argv0);
        assert(std::string {args[2]} == "-v");
        assert(args.size() == 3);
    }

    static auto test_required(const Arguments::ArgumentSpan& args) -> void
    {
        print(args, "Required");
        assert(std::string {args[0]} == "one");
        assert(std::string {args[1]} == "two");
        assert(std::string {args[2]} == "three");
        assert(args.size() == 3);
    }

    static auto test_view(const Arguments::ArgumentSpan& args,
                          const char* argv0) -> void
    {
        print(args, "View");
        assert(std::string {args[0]} == argv0);
        assert(std::string {args[1]} == "-f");
        assert(std::string {args[2]} == argv0);
        assert(std::string {args[3]} == "-v");
        assert(std::string {args[4]} == "one");
        assert(std::string {args[5]} == "two");
        assert(std::string {args[6]} == "three");
    }
}

auto main(int argc, char* argv[]) -> int
{
    using namespace TestArguments;

    try {
        assert(argc > 0);
        assert(*argv != nullptr);
        ArgumentData data {get_strings(*argv)};
        Arguments args {data.size(), data.data()};
        std::string filename;
        bool verbose {false};
        parse(filename, verbose, args);
        test_optional(args.optional(), *argv);
        test_required(args.required());
        test_view(args.view(), *argv);
        assert(filename == *argv);
        assert(verbose);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
