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

#include "network/create-socketresult.h"        // create_socketresult()
#include "network/create-socket-handle.h"       // create_socket()
#include "network/format-os-error.h"            // format_os_error()
#include "network/format.h"                     // Format
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/handle-null.h"                // handle_null
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/socketfamily.h"               // SocketFamily
#include "network/sockethints.h"                // SocketHints
#include "network/socketprotocol.h"             // SocketProtocol
#include "network/socketresult.h"               // SocketResult
#include "network/sockettype.h"                 // SocketType
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // ::socket()
#else
#include <sys/socket.h>     // ::socket()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::create_socketresult(const SocketHints& hints,
                                  bool is_verbose) -> SocketResult
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    if (is_verbose) {
        std::cout << "Calling ::socket("
                  << Format("domain")
                  << SocketFamily(hints.m_family)
                  << Format(delim, tab, "type")
                  << SocketType(hints.m_socktype)
                  << Format(delim, tab, "protocol")
                  << SocketProtocol(hints.m_protocol, hints.m_family)
                  << ')'
                  << std::endl;
    }

    reset_last_context_error();
    const auto handle {::socket(hints.m_family,
                                hints.m_socktype,
                                hints.m_protocol)};

    if (handle == handle_null) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::socket("
            << Format("domain")
            << SocketFamily(hints.m_family)
            << Format(delim, tab, "type")
            << SocketType(hints.m_socktype)
            << Format(delim, tab, "protocol")
            << SocketProtocol(hints.m_protocol, hints.m_family)
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return create_socket(hints.m_family, handle, is_verbose);
}
