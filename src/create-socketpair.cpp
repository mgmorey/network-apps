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

#ifndef WIN32

#include "network/create-socketpair.h"          // create_socketpair()
#include "network/always-false.h"               // always_false_v
#include "network/create-socketpairresult.h"    // create_socketpairresult()
#include "network/error-strings.h"              // VISITOR_ERROR
#include "network/error.h"                      // Error
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/sockethints.h"                // SocketHints
#include "network/socketpair.h"                 // SocketPair

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::create_socketpair(const SocketHints& hints,
                                bool is_verbose) -> SocketPair
{
    SocketPair socketpair;
    auto result {create_socketpairresult(hints, is_verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, SocketPair>) {
            socketpair[0].swap(arg[0]);
            socketpair[1].swap(arg[1]);
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            throw Error(arg.string());
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, result);
    return socketpair;
}

#endif
