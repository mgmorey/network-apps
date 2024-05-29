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

#include "network/get-endpointresult.h"         // get_endpoint()
#include "network/addressstring.h"              // AddressString
#include "network/buffer.h"                     // Buffer
#include "network/bytestring.h"                 // ByteString
#include "network/endpoint.h"                   // Endpoint
#include "network/endpointresult.h"             // EndpointResult
#include "network/format-ai-error.h"            // format_ai_error()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/hostname.h"                   // hostname_size_max
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/service.h"                    // service_size_max
#include "network/to-os-error.h"                // to_os_error()

#ifdef WIN32
#include <ws2tcpip.h>   // ::getnameinfo()
#else
#include <netdb.h>      // ::getnameinfo()
#endif

#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream

auto Network::get_endpointresult(const ByteString& addr, int flags,
                                 bool verbose) -> EndpointResult
{
    const auto addr_len {get_sa_length(addr)};
    const auto* const addr_ptr {get_sa_pointer(addr)};
    const AddressString addr_str {addr};
    Buffer<char> hostname {hostname_size_max};
    Buffer<char> service {service_size_max};

    if (verbose) {
        std::cout << "Calling ::getnameinfo("
                  << addr_str
                  << ", "
                  << addr_len
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    if (const auto error {::getnameinfo(addr_ptr, addr_len,
                                        hostname.data(), hostname.size(),
                                        service.data(), service.size(),
                                        flags)}) {
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::getnameinfo("
            << addr_str
            << ", "
            << addr_len
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << format_ai_error(error)
            << ')';
        return OsErrorResult {os_error, oss.str()};
    }

    return Endpoint {hostname, service};
}
