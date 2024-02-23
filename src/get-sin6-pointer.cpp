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

#include "network/get-sin6-pointer.h"           // get_sin6_pointer()
#include "network/bytestring.h"                 // ByteString
#include "network/familyerror.h"                // FamilyError
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/sin6-sizes.h"                 // sin6_size
#include "network/sin6lengtherror.h"            // Sin6LengthError

#ifdef WIN32
#include <winsock2.h>       // AF_INET6
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#include <sys/socket.h>     // AF_INET6
#endif

#include <string>   // std::to_string()

namespace {
    auto validate_sin6(const Network::ByteString& addr) -> void
    {
        if (Network::get_sa_family(addr) != AF_INET6) {
            throw Network::FamilyError();
        }

        if (addr.size() != Network::sin6_size) {
            throw Network::Sin6LengthError(std::to_string(addr.size()),
                                           Network::sin6_size,
                                           Network::sin6_size);
        }
    }
}

auto Network::get_sin6_pointer(const ByteString& addr) -> const sockaddr_in6*
{
    validate_sin6(addr);
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in6*>(pointer);
}

// cppcheck-suppress constParameterReference
auto Network::get_sin6_pointer(ByteString& addr) -> sockaddr_in6*
{
    validate_sin6(addr);
    void* pointer = addr.data();
    return static_cast<sockaddr_in6*>(pointer);
}
