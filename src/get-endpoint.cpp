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

#include "network/get-endpoint.h"       // Endpoint, EndpointResult,
                                        // Result, get_endpoint()
#include "network/buffer.h"             // Buffer
#include "network/get-length.h"         // Bytes, get_length()
#include "network/get-sa-pointer.h"     // Bytes, get_sa_pointer()
#include "network/is-valid.h"           // Bytes, is_valid()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#endif

#include <algorithm>    // std::max()
#include <cassert>      // assert()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::get_endpoint(const Bytes& addr, int flags, bool verbose) ->
    Network::EndpointResult
{
    Buffer host {NI_MAXHOST};
    Buffer service {NI_MAXSERV};
    assert(is_valid(addr, verbose));  // NOLINT
    const auto addr_len {get_length(addr)};
    const auto *const addr_ptr {get_sa_pointer(addr)};

    if (verbose) {
        std::cerr << "Calling getnameinfo("
                  << addr
                  << ", "
                  << addr_len
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    const auto error {::getnameinfo(addr_ptr, addr_len,
                                    host.data(), host.size(),
                                    service.data(), service.size(),
                                    flags)};

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << addr
            << ", "
            << addr_len
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << ::gai_strerror(error)
            << ')';
        return IntegerResult {error, oss.str()};
    }

    return Endpoint {host, service};
}
