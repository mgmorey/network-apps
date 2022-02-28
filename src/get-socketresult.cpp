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

#include "network/get-socketresult.h"   // Fd, FdResult,
                                        // OsErrorResult, Socket,
                                        // fd_null, get_socketresult(),
                                        // operator<<()
#include "network/format.h"             // Format
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()

#ifdef WIN32
#include <winsock2.h>       // socket()
#else
#include <sys/socket.h>     // socket()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_socketresult(const SocketHints& hints,
                               bool pending,
                               bool verbose) noexcept -> Network::FdResult
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    if (verbose) {
        std::cout << "Calling ::socket("
                  << Format("domain")
                  << hints.family()
                  << Format(delim, tab, "type")
                  << hints.socktype()
                  << Format(delim, tab, "protocol")
                  << hints.protocol()
                  << ')'
                  << std::endl;
    }

    reset_last_os_error();
    const auto handle {::socket(hints.family(), hints.socktype(), hints.protocol())};

    if (handle == fd_null) {
        const auto error = get_last_os_error();
        std::ostringstream oss;
        oss << "Call to ::socket("
            << Format("domain")
            << hints.family()
            << Format(delim, tab, "type")
            << hints.socktype()
            << Format(delim, tab, "protocol")
            << hints.protocol()
            << ") failed with error "
            << error
            << ": "
            << format_os_error(error);
        return OsErrorResult {error, oss.str()};
    }

    return Fd {handle, pending, verbose};
}
