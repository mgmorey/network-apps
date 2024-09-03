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

#include "network/open-socket.h"                // open()
#include "network/addresserror.h"               // AddressError
#include "network/addressstring.h"              // AddressString
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/openhandler.h"                // OpenHandler
#include "network/opensocketparams.h"           // OpenSocketParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/sa-length-limits.h"           // sa_length_min
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::open(const OpenHandler& handler,
                   const OpenSocketParams& args) -> OsErrorResult
{
    const auto handle {args.socket->handle()};
    const AddressString addr_str {args.addr};
    const auto [addr_ptr, addr_len] {get_sa_span(args.addr)};

    if (addr_len == sa_length_min) {
        throw AddressError("Address payload length is zero: " +
                           std::string {addr_str});
    }

    if (args.socket->is_verbose()) {
        std::cout << "Calling "
                  << handler.second
                  << '('
                  << handle
                  << ", "
                  << addr_str
                  << ", "
                  << addr_len
                  << ')'
                  << std::endl;
    }

    reset_last_context_error();

    if (handler.first(handle, addr_ptr, addr_len) == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
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
