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
#include "network/create-socketresult.hpp"      // create_socketresult()
#include "network/error.hpp"                    // Error
#include "network/run.hpp"                      // run()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/sockethints.hpp"              // SocketHints
#include "network/uniquesocket.hpp"             // UniqueSocket

#include <utility>      // std::move()

auto Network::create_socket(const SocketHints& hints,
                            const SharedRuntime& sr) -> UniqueSocket
{
    auto result {create_socketresult(hints, sr)};

    if (!result) {
        throw Error(result.error().string());
    }

    return std::move(*result);
}

auto Network::create_socket(const SocketHints& hints,
                            bool is_verbose) -> UniqueSocket
{
    return create_socket(hints, run(is_verbose));
}
