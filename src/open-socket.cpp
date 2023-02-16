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

#include "network/open-socket.h"        // ByteString, OpenHandler,
                                        // OsErrorResult, Socket,
                                        // descriptor_type, open(),
                                        // operator<<()
#include "network/context-error.h"      // get_last_context_error(),
                                        // reset_last_context_error()
#include "network/format-bytestring.h"  // format(), OptionalString
#include "network/format-os-error.h"    // format_os_error()
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/socket-error.h"       // socket_error
#include "network/to-integer.h"         // to_integer()
#include "network/to-os-error.h"        // to_os_error()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::open(const OpenHandler& handler,
                   const OpenSocketParams& args) -> Network::OsErrorResult
{
    const auto* const pointer {get_sa_pointer(args.addr)};
    const auto length {get_length(args.addr)};
    reset_last_context_error();
    OptionalString addr_str;

    if (args.verbose) {
        std::cout << "Calling "
                  << handler.second
                  << '('
                  << args.socket
                  << ", "
                  << format(args.addr, addr_str)
                  << ", "
                  << to_integer(length)
                  << ')'
                  << std::endl;
    }

    const descriptor_type handle {args.socket};

    if (handler.first(handle, pointer, length) == socket_error) {
        const auto error = get_last_context_error();
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << args.socket
            << ", "
            << format(args.addr, addr_str)
            << ", "
            << to_integer(length)
            << ") failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    return OsErrorResult {};
}
