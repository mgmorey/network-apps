// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/open-handle.h"                // open()
#include "network/addresserror.h"               // AddressError
#include "network/addressstring.h"              // AddressString
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-api-error.h"              // get_api_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/openhandler.h"                // OpenHandler
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-api-error.h"            // reset_api_error()
#include "network/sa-length-limits.h"           // sa_length_min
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <winsock2.h>       // ::bind(), ::connect()
#else
#include <sys/socket.h>     // ::bind(), ::connect()
#endif

#include <array>        // std::arrray
#include <cstddef>      // std::size_t
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <utility>      // std::make_pair

namespace
{
    auto get_binding(bool is_bind) -> Network::OpenHandler
    {
        static const std::array<Network::OpenHandler, 2> bindings {
            std::make_pair(::connect, "::connect"),
            std::make_pair(::bind, "::bind"),
        };

        return bindings.at(static_cast<std::size_t>(is_bind));
    }
}

auto Network::open(const OpenHandleParams& args, bool is_bind) -> OsErrorResult
{
    const auto binding {get_binding(is_bind)};
    const auto handle {args.m_handle};
    const AddressString addr_str {args.m_addr};
    const auto [addr_ptr, addr_len] {get_sa_span(args.m_addr)};

    if (addr_len == sa_length_min) {
        throw AddressError("Address payload length is zero: " +
                           std::string {addr_str});
    }

    if (args.m_is_verbose) {
        std::cout << "Calling "
                  << binding.second
                  << '('
                  << handle
                  << ", "
                  << addr_str
                  << ", "
                  << addr_len
                  << ')'
                  << std::endl;
    }

    reset_api_error();

    if (binding.first(handle, addr_ptr, addr_len) == socket_error) {
        const auto error {get_api_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << binding.second
            << '('
            << handle
            << ", "
            << addr_str
            << ", "
            << addr_len
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
