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

#include "network/get-endpointresult.h" // ByteString, Endpoint,
                                        // EndpointResult,
                                        // OsErrorResult,
                                        // get_endpoint(),
                                        // hostname_size_max,
                                        // service_size_max
#include "network/addresserror.h"       // AddressError
#include "network/buffer.h"             // Buffer
#include "network/bytestring.h"         // ByteString
#include "network/format-ai-error.h"    // format_ai_error()
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/is-valid.h"           // is_valid()
#include "network/to-os-error.h"        // to_os_error()
#include "network/to-string.h"          // to_string()

#ifdef WIN32
#include <ws2tcpip.h>   // ::getnameinfo()
#else
#include <netdb.h>      // ::getnameinfo()
#endif

#include <algorithm>    // std::max()
#include <iostream>     // std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::get_endpointresult(const ByteString& addr, int flags,
                                 bool verbose) ->
    Network::EndpointResult
{
    if (!is_valid(addr)) {
        std::ostringstream oss;
        oss << "Invalid socket address: "
            << to_string(addr);
        return OsErrorResult {0, oss.str()};
    }

    Buffer host {hostname_size_max};
    Buffer service {service_size_max};
    const auto length {get_length(addr)};
    const auto *const pointer {get_sa_pointer(addr)};

    if (verbose) {
        std::cout << "Calling ::getnameinfo("
                  << addr
                  << ", "
                  << length
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    if (const auto error {::getnameinfo(pointer, length,
                                        host.data(), host.size(),
                                        service.data(), service.size(),
                                        flags)}) {
        const auto os_error {to_os_error(error)};
        std::ostringstream oss;
        oss << "Call to ::getnameinfo("
            << addr
            << ", "
            << length
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << format_ai_error(error)
            << ')';
        return OsErrorResult {os_error, oss.str()};
    }

    return Endpoint {host, service};
}
