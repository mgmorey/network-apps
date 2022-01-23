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
                                        // ErrorResult, get_endpoint()
#include "network/addresserror.h"       // AddressError
#include "network/buffer.h"             // Buffer
#include "network/bytestring.h"         // ByteString
#include "network/get-length.h"         // get_length()
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/is-valid.h"           // is_valid()

#ifdef _WIN32
#include <ws2tcpip.h>   // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#else
#include <netdb.h>      // NI_MAXHOST, NI_MAXSERV, gai_strerror(),
                        // getnameinfo()
#endif

#include <algorithm>    // std::max()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream
#include <string>       // std::string

auto Network::get_endpoint(const ByteString& str, int flags, bool verbose) ->
    Network::EndpointResult
{
    if (!is_valid(str)) {
        throw AddressError(str);
    }

    Buffer host {NI_MAXHOST};
    Buffer service {NI_MAXSERV};
    const auto length {get_length(str)};
    const auto *const pointer {get_sa_pointer(str)};

    if (verbose) {
        std::cerr << "Calling getnameinfo("
                  << str
                  << ", "
                  << length
                  << ", ..., "
                  << flags
                  << ')'
                  << std::endl;
    }

    const auto error {::getnameinfo(pointer, length,
                                    host.data(), host.size(),
                                    service.data(), service.size(),
                                    flags)};

    if (error != 0) {
        std::ostringstream oss;
        oss << "Call to getnameinfo("
            << str
            << ", "
            << length
            << ", ..., "
            << flags
            << ") returned "
            << error
            << " ("
            << ::gai_strerror(error)
            << ')';
        return ErrorResult {error, oss.str()};
    }

    return Endpoint {host, service};
}
