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

#include "network/create-socket-hints.hpp"      // create_socket()
#include "network/always-false.hpp"             // always_false_v
#include "network/create-socketresult.hpp"      // create_socketresult()
#include "network/error-strings.hpp"            // VISITOR_ERROR
#include "network/error.hpp"                    // Error
#include "network/oserrorresult.hpp"            // OsErrorResult
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/sockethints.hpp"              // SocketHints
#include "network/uniquesocket.hpp"             // UniqueSocket

#include <type_traits>  // std::decay_t, std::is_same_v
#include <variant>      // std::visit()

auto Network::create_socket(const SocketHints& hints,
                            const SharedRuntime& sr) -> UniqueSocket
{
    UniqueSocket sock;
    auto result {create_socketresult(hints, sr)};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, UniqueSocket>) {
            sock.swap(arg);
        }
        else if constexpr (std::is_same_v<T, OsErrorResult>) {
            throw Error(arg.string());
        }
        else {
            static_assert(always_false_v<T>, VISITOR_ERROR);
        }
    }, result);
    return sock;
}

auto Network::create_socket(const SocketHints& hints,
                            bool is_verbose) -> UniqueSocket
{
    const auto rt {run(is_verbose)};
    return create_socket(hints, rt);
}
