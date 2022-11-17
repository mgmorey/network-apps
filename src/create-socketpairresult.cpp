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

#include "network/create-socketpairresult.h"    // OsErrorResult,
                                                // Socket, SocketPair,
                                                // SocketPairResult,
                                                // create_socketpair(),
                                                // descriptor_null,
                                                // descriptor_type,
                                                // operator<<()
#include "network/context-error.h"              // get_last_context_error(),
                                                // reset_last_context_error()
#include "network/format-os-error.h"            // format_os_error()
#include "network/format.h"                     // Format
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#ifndef WIN32

#include <sys/socket.h>     // socketpair()


#include <array>        // std::array
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::create_socketpairresult(const SocketHints& hints,
                                      bool verbose) noexcept ->
    Network::SocketPairResult
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    std::array<descriptor_type, 2> handles {descriptor_null, descriptor_null};

    if (verbose) {
        std::cout << "Calling ::socketpair("
                  << Format("domain")
                  << hints.family()
                  << Format(delim, tab, "type")
                  << hints.socktype()
                  << Format(delim, tab, "protocol")
                  << hints.protocol()
                  << ", ...)"
                  << std::endl;
    }

    reset_last_context_error();

    if (::socketpair(hints.family(),
                     hints.socktype(),
                     hints.protocol(),
                     handles.data()) == socket_error) {
        const auto error = get_last_context_error();
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::socketpair("
            << Format("domain")
            << hints.family()
            << Format(delim, tab, "type")
            << hints.socktype()
            << Format(delim, tab, "protocol")
            << hints.protocol()
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return SocketPair {
        Socket {handles[0], false, verbose},
        Socket {handles[1], false, verbose}
    };
}

#endif
