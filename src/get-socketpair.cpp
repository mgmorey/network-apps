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
                                        // OsErrorResult, Socket,
                                        // fd_null, fd_type,
                                        // get_socketpair(),
                                        // operator<<()
#include "network/format.h"             // Format
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/os-features.h"        // WIN32
#include "network/socket-error.h"       // socket_error

#ifndef WIN32

#include <sys/socket.h>     // socketpair()


#include <array>        // std::array
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_socketpair(const Hints& hints,
                             bool verbose) -> Network::FdPairResult
{
    constexpr auto delim {", "};
    constexpr auto tab {0};

    std::array<fd_type, 2> fds {fd_null, fd_null};

    if (verbose) {
        std::cerr << "Calling socketpair("
                  << Format("domain")
                  << hints.family()
                  << Format(delim, tab, "type")
                  << hints.socktype()
                  << Format(delim, tab, "protocol")
                  << hints.protocol()
                  << ", ...)"
                  << std::endl;
    }

    reset_last_os_error();

    if (::socketpair(hints.family(),
                     hints.socktype(),
                     hints.protocol(),
                     fds.data()) == socket_error) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to socketpair("
            << Format("domain")
            << hints.family()
            << Format(delim, tab, "type")
            << hints.socktype()
            << Format(delim, tab, "protocol")
            << hints.protocol()
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return FdPair {Fd {fds[0]}, Fd {fds[1]}};
}

#endif
