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

#include "network/get-socket.h"         // Fd, FdResult,
                                        // OsErrorResult, Socket,
                                        // fd_null, get_socket(),
                                        // operator<<()
#include "network/format.h"             // Format
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()

#ifdef _WIN32
#include <winsock2.h>       // socket()
#else
#include <sys/socket.h>     // socket()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_socket(const Socket& sock,
                         bool verbose) -> Network::FdResult
{
    constexpr auto delim {", "};
    constexpr auto tab {0};

    if (verbose) {
        std::cerr << "Calling socket("
                  << Format("domain")
                  << sock.family()
                  << Format(delim, tab, "type")
                  << sock.socktype()
                  << Format(delim, tab, "protocol")
                  << sock.protocol()
                  << ')'
                  << std::endl;
    }

    reset_last_os_error();
    const auto fd {::socket(sock.family(), sock.socktype(), sock.protocol())};

    if (fd == fd_null) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to socket("
            << Format("domain")
            << sock.family()
            << Format(delim, tab, "type")
            << sock.socktype()
            << Format(delim, tab, "protocol")
            << sock.protocol()
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return Fd {fd};
}
