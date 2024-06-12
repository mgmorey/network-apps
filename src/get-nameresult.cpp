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

#include "network/get-nameresult.h"             // get_nameresult()
#include "network/addressstring.h"              // AddressString
#include "network/buffer.h"                     // Buffer
#include "network/byte.h"                       // Byte
#include "network/bytestring.h"                 // ByteString
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/get-sa-span.h"                // get_sa_span()
#include "network/getnamehandler.h"             // GetNameHandler
#include "network/getnameparams.h"              // GetNameParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_context_last_error()
#include "network/sa-length-limits.h"           // sa_length_max
#include "network/socket-error.h"               // socket_error
#include "network/socket-type.h"                // socket_type
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-size.h"                    // to_size()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_nameresult(const GetNameHandler& handler,
                             const GetNameParams& args) -> ByteStringResult
{
    const socket_type handle {args.handle};
    Buffer<Byte> buffer {sa_length_max};
    const AddressString addr_str {ByteString {buffer}};
    auto [addr_ptr, addr_len] {get_sa_span(buffer)};

    if (args.verbose) {
        std::cout << "Calling "
                  << handler.second
                  << '('
                  << handle
                  << ", "
                  << addr_str
                  << ", "
                  << addr_len
                  << ", ...)"
                  << std::endl;
    }

    reset_last_context_error();

    if (handler.first(handle, addr_ptr, &addr_len) == socket_error) {
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
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        return OsErrorResult {os_error, oss.str()};
    }

    buffer.resize(to_size(addr_len));
    return ByteString {buffer};
}
