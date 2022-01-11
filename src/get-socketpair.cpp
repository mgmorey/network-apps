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

#include "network/get-socketpair.h"     // FdPair, FdPairResult,
                                        // Result, Socket, fd_null,
                                        // fd_type, get_socketpair(),
                                        // operator<<()
#include "network/format.h"             // Format
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error

#ifndef _WIN32

#include <sys/socket.h>     // socketpair()


#include <array>        // std::array
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_socketpair(const Socket& sock,
                             bool verbose) -> Network::FdPairResult
{
    constexpr auto delim {", "};
    constexpr auto tab {0};

    Result result;
    std::array<fd_type, 2> fds {fd_null, fd_null};

    if (verbose) {
        std::cerr << "Calling socketpair("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ", ...)"
                  << std::endl;
    }

    reset_last_os_error();
    auto code {::socketpair(sock.family(),
                            sock.socktype(),
                            sock.protocol(),
                            fds.data())};

    if (code == socket_error) {
        auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(error);
        return Result(error, oss.str());
    }

    return FdPair {fds[0], fds[1]};
}

#endif
