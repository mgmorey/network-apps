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
#include "network/bytestringresult.h"           // ByteStringResult
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/getnamehandler.h"             // GetNameHandler
#include "network/getnameparams.h"              // GetNameParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/reset-last-context-error.h"   // reset_context_last_error()
#include "network/sa-len-limits.h"              // sa_len_max
#include "network/socket-error.h"               // socket_error
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-size.h"                    // to_size()
#include "network/to-socket-length.h"           // to_socket_length()

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_nameresult(const GetNameHandler& handler,
                             const GetNameParams& args) -> ByteStringResult
{
    Buffer<Byte> addr(sa_len_max);
    auto* addr_ptr {get_sa_pointer(addr)};
    auto addr_len {to_socket_length(addr.size())};
    const AddressString addr_str {addr};

    if (args.verbose) {
        std::cout << "Calling "
                  << handler.second
                  << '('
                  << args.handle
                  << ", "
                  << addr_str
                  << ", "
                  << addr_len
                  << ", ...)"
                  << std::endl;
    }

    reset_last_context_error();

    if (handler.first(args.handle, addr_ptr, &addr_len) == socket_error) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to "
            << handler.second
            << '('
            << args.handle
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

    addr.resize(to_size(addr_len));
    return addr;
}
