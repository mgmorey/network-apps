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

#include "network/create-socketresult.hpp"      // create_socketresult()
#include "network/create-socket-handle.hpp"     // create_socket()
#include "network/format-os-error.hpp"          // format_os_error()
#include "network/format.hpp"                   // Format
#include "network/get-api-error.hpp"            // get_api_error()
#include "network/handle-null.hpp"              // handle_null
#include "network/oserror.hpp"                  // OsError
#include "network/reset-api-error.hpp"          // reset_api_error()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/socketfamily.hpp"             // SocketFamily
#include "network/sockethints.hpp"              // SocketHints
#include "network/socketprotocol.hpp"           // SocketProtocol
#include "network/socketresult.hpp"             // SocketResult
#include "network/sockettype.hpp"               // SocketType
#include "network/to-os-error.hpp"              // to_os_error()

#ifdef _WIN32
#include <winsock2.h>       // ::socket()
#else
#include <sys/socket.h>     // ::socket()
#endif

#include <expected>     // std::unexpected
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::create_socketresult(const SocketHints& hints,
                                  const SharedRuntime& sr) -> SocketResult
{
    static constexpr auto delim {", "};
    static constexpr auto tab {0};

    const auto family {hints.m_family};

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Calling ::socket("
                  << Format("domain")
                  << SocketFamily(family)
                  << Format(delim, tab, "type")
                  << SocketType(hints.m_socktype)
                  << Format(delim, tab, "protocol")
                  << SocketProtocol(hints.m_protocol, family)
                  << ')'
                  << std::endl;
        // clang-format on
    }

    reset_api_error();
    const auto handle {::socket(family,
                                hints.m_socktype,
                                hints.m_protocol)};

    if (handle == handle_null) {
        const auto api_error {get_api_error()};
        const auto os_error {to_os_error(api_error)};
        std::ostringstream oss;
        // clang-format off
        oss << "Call to ::socket("
            << Format("domain")
            << SocketFamily(family)
            << Format(delim, tab, "type")
            << SocketType(hints.m_socktype)
            << Format(delim, tab, "protocol")
            << SocketProtocol(hints.m_protocol, family)
            << ") failed with error "
            << api_error
            << ": "
            << format_os_error(os_error);
        // clang-format on
        return std::unexpected {OsError {os_error, oss.str()}};
    }

    if (sr->is_verbose()) {
        // clang-format off
        std::cout << "Call to ::socket("
                  << Format("domain")
                  << SocketFamily(family)
                  << Format(delim, tab, "type")
                  << SocketType(hints.m_socktype)
                  << Format(delim, tab, "protocol")
                  << SocketProtocol(hints.m_protocol, family)
                  << ") returned data "
                  << handle
                  << std::endl;
        // clang-format on
    }

     return create_socket(handle, family, sr->is_verbose());
}
