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
#include "network/network.hpp"          // RuntimeError
                                        // get_api_error()
                                        // get_os_error()
                                        // reset_api_error()
                                        // reset_os_error()
#include "network/parse.hpp"            // parse()

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::cout, std::endl

namespace
{
    using Network::Error;
    using Network::LogicError;
    using Network::RuntimeError;
    using Network::get_api_error;
    using Network::get_os_error;
    using Network::parse;
    using Network::reset_api_error;
    using Network::reset_os_error;

    auto is_verbose {false};  // NOLINT

    auto parse_arguments(int argc, char** argv) -> void
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
            is_verbose = true;
        }

        static_cast<void>(_);
    }

    auto print(const Error& error) -> void
    {
        if (is_verbose) {
            std::cout << "Exception: "
                      << error.what()
                      << std::endl;
        }
    }

    auto test_api_error() -> void
    {
        reset_api_error();
        assert(get_api_error() == 0);
    }

    auto test_os_error() -> void
    {
        reset_os_error();
        assert(get_os_error() == 0);
    }

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
        test_api_error();
        test_os_error();
        test_throw_error();
        test_throw_logic_error();
        test_throw_runtime_error();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}
