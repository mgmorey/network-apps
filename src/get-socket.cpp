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

#include "network/get-socket.h"         // Fd, OsErrorResult,
                                        // SocketHints, get_socket()
#include "network/error.h"              // Error
#include "network/get-socketresult.h"   // FdResult,
                                        // get_socketresult()
#include "network/overloaded.h"         // Overloaded

#include <variant>      // std::visit()

auto Network::get_socket(const SocketHints& hints, bool verbose) -> Network::Fd
{
    Fd result;
    const auto fd_result {get_socketresult(hints, verbose)};
    std::visit(Overloaded {
            [&](Fd fd) {
                result = fd;
            },
            [&](const OsErrorResult& error) {
                throw Error(error.string());
            }
        }, fd_result);
    return result;
}
