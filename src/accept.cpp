// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/accept.h"                     // accept()
#include "network/addressstring.h"              // AddressString
#include "network/buffer.h"                     // Buffer
#include "network/byte.h"                       // Byte
#include "network/bytestring.h"                 // ByteString
#include "network/descriptor-null.h"            // descriptor_null
#include "network/descriptor-type.h"            // descriptor_type
#include "network/error.h"                      // Error
#include "network/format-os-error.h"            // format_os_error()
#include "network/get-last-context-error.h"     // get_last_context_error()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/reset-last-context-error.h"   // reset_last_context_error()
#include "network/sa-len-limits.h"              // sa_len_max
#include "network/socket.h"                     // Socket
#include "network/to-os-error.h"                // to_os_error()
#include "network/to-size.h"                    // to_size()

#ifdef WIN32
#include <winsock2.h>       // ::accept()
#else
#include <sys/socket.h>     // ::accept()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <utility>      // std::pair

auto Network::accept(const Socket& sock, bool verbose) ->
    std::pair<Socket, ByteString>
{
    Buffer<Byte> addr {sa_len_max};
    auto* addr_ptr {get_sa_pointer(addr)};
    auto addr_len {get_sa_length(addr)};
    const AddressString addr_str {ByteString {addr}};

    if (verbose) {
        std::cout << "Calling ::accept("
                  << addr_str
                  << ", "
                  << addr_len
                  << ", ...)"
                  << std::endl;
    }

    reset_last_context_error();
    const auto handle_1 {sock.handle()};
    const auto handle_2 {::accept(handle_1, addr_ptr, &addr_len)};

    if (handle_2 == descriptor_null) {
        const auto error {get_last_context_error()};
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::accept("
            << handle_1
            << ", "
            << addr_str
            << ", "
            << addr_len
            << ", ...) failed with error "
            << error
            << ": "
            << format_os_error(os_error);
        throw Error(oss.str());
    }

    addr.resize(to_size(addr_len));
    return {Socket {handle_2, verbose}, ByteString {addr}};
}
