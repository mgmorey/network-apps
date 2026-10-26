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

#include "network/get-option.hpp"       // get_optarg(), get_opterr(),
                                        // get_optind(), get_option(),
                                        // get_optopt()
#include "network/argumentspan.hpp"     // ArgumentSpan
#include "network/logicerror.hpp"       // LogicError

#ifndef _WIN32
#include <unistd.h>         // NOLINT getopt(), optarg, opterr,
                            // optind, optopt
#else
#include <cstring>          // std::strchr()
#endif

#ifdef _WIN32
namespace
{
    auto optarg = static_cast<char*>(nullptr);
    auto opterr = 1;
    auto optind = 1;
    auto optopt = 0;
    auto next_option = 1;

    auto get_option(const Network::ArgumentSpan& args, const char* str) -> int
    {
        optarg = nullptr;

        if (optind >= static_cast<int>(args.size())) {
            return -1;
        }

        auto* argument = args[static_cast<std::size_t>(optind)];
        if (next_option == 1) {
            if (argument[0] != '-' || argument[1] == '\0') {
                return -1;
            }
            if (argument[1] == '-' && argument[2] == '\0') {
                ++optind;
                return -1;
            }
        }

        const auto option {static_cast<unsigned char>(argument[next_option])};
        const auto* option_definition {std::strchr(str, option)};
        if (option == ':' || option_definition == nullptr) {
            optopt = option;
            if (argument[++next_option] == '\0') {
                ++optind;
                next_option = 1;
            }
            return '?';
        }

        if (option_definition[1] != ':') {
            if (argument[++next_option] == '\0') {
                ++optind;
                next_option = 1;
            }
            return option;
        }

        if (argument[next_option + 1] != '\0') {
            optarg = argument + next_option + 1;
            ++optind;
            next_option = 1;
            return option;
        }

        ++optind;
        next_option = 1;
        if (optind < static_cast<int>(args.size())) {
            optarg = args[static_cast<std::size_t>(optind++)];
            return option;
        }

        optopt = option;
        return str[0] == ':' ? ':' : '?';
    }
}
#endif

auto Network::get_optarg() noexcept -> char*
{
#ifdef _WIN32
    return ::optarg;
#else
    return ::optarg;  // NOLINT
#endif
}

auto Network::get_opterr() noexcept -> int
{
#ifdef _WIN32
    return ::opterr;
#else
    return ::opterr;  // NOLINT
#endif
}

auto Network::get_optind() noexcept -> int
{
#ifdef _WIN32
    return ::optind;
#else
    return ::optind;  // NOLINT
#endif
}

auto Network::get_option(const ArgumentSpan& args, const char* str) -> int
{
    if (str == nullptr || *str == '\0') {
        throw LogicError {"No command-line options to parse"};
    }

#ifdef _WIN32
    return ::get_option(args, str);
#else
    return ::getopt(static_cast<int>(args.size()), args.data(), str);
#endif
}

auto Network::get_optopt() noexcept -> int
{
#ifdef _WIN32
    return ::optopt;
#else
    return ::optopt;  // NOLINT
#endif
}
