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

#include "network/create-socketpairresult.h"    // create_socketpairresult()
#include "network/create-socket-handle.h"       // create_socket()
#include "network/format-os-error.h"            // format_os_error()
#include "network/format.h"                     // Format
#include "network/get-api-error.h"              // get_api_error()
#include "network/handle-null.h"                // handle_null
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-api-error.h"            // reset_api_error()
#include "network/socket-error.h"               // socket_error
#include "network/socketfamily.h"               // SocketFamily
#include "network/sockethints.h"                // SocketHints
#include "network/socketpair.h"                 // SocketPair
#include "network/socketpairresult.h"           // SocketPairResult
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/sockettype.h"                 // SocketType
#include "network/to-os-error.h"                // to_os_error()

#include <sys/socket.h>     // ::socketpair()

#include <array>        // std::array
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::create_socketpairresult(const SocketHints& hints,
                                      bool is_verbose) -> SocketPairResult
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    const auto family {hints.m_family};
    std::array<handle_type, 2> handles {handle_null, handle_null};

    if (is_verbose) {
        std::cout << "Calling ::socketpair("
                  << Format("domain")
                  << SocketFamily(family)
                  << Format(delim, tab, "type")
                  << SocketType(hints.m_socktype)
                  << Format(delim, tab, "protocol")
                  << SocketProtocol(hints.m_protocol, family)
                  << ", ...)"
                  << std::endl;
    }

    reset_api_error();

    if (::socketpair(family,
                     hints.m_socktype,
                     hints.m_protocol,
                     handles.data()) == socket_error) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        oss << "Call to ::socketpair("
            << Format("domain")
            << SocketFamily(family)
            << Format(delim, tab, "type")
            << SocketType(hints.m_socktype)
            << Format(delim, tab, "protocol")
            << SocketProtocol(hints.m_protocol, family)
            << ", ...) failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return SocketPair {
        create_socket(handles[0], family, is_verbose),
        create_socket(handles[1], family, is_verbose)
    };
}

#endif
