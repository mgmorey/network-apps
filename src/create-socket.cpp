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

#include "network/create-socket.h"              // create_socket()
#include "network/create-socketresult.h"        // SocketResult,
                                                // create_socketresult()
#include "network/descriptor-null.h"            // descriptor_null
#include "network/error.h"                      // Error
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/overloaded.h"                 // Overloaded
#include "network/socket.h"                     // Socket
#include "network/sockethints.h"                // SocketHints

#include <variant>      // std::visit()

auto Network::create_socket(const SocketHints& hints,
                            bool pending,
                            bool verbose) -> Network::Socket
{
    Socket result {descriptor_null, pending, verbose};
    const auto socket_result {create_socketresult(hints, pending, verbose)};
    std::visit(Overloaded {
            [&](const Socket& sock) {
                result = sock;
            },
            [&](const OsErrorResult& error) {
                throw Error(error.string());
            }
        }, socket_result);
    return result;
}
