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

#include <optional>     // std::optional
#include <string>       // std::string
#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

template <class>
inline constexpr bool always_false_v {false};

auto Network::create_socketpair(const SocketHints& hints,
                                bool verbose) -> SocketPair
{
    std::string result_string;
    std::optional<SocketPair> result_socketpair;
    const auto socketpair_result {create_socketpairresult(hints, verbose)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, SocketPair>) {
            result_socketpair = arg;
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            result_string = arg.string();
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, socketpair_result);

    if (!result_socketpair) {
        throw Error(result_string);
    }

    return *result_socketpair;
}

#endif
