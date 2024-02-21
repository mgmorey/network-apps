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

#include "network/get-sin-pointer.h"            // get_sin_pointer()
#include "network/bytestring.h"                 // ByteString
#include "network/familyerror.h"                // FamilyError
#include "network/get-sa-family.h"              // get_sa_family()
#include "network/salengtherror.h"              // SaLengthError
#include "network/sin-sizes.h"                  // sin_size

#ifdef WIN32
#include <winsock2.h>       // AF_INET, sockaddr_in
#else
#include <netinet/in.h>     // sockaddr_in
#include <sys/socket.h>     // AF_INET
#endif

#include <string>   // std::to_string()

namespace {
    auto validate_sin(const Network::ByteString& addr) -> void
    {
        if (Network::get_sa_family(addr) != AF_INET) {
            throw Network::FamilyError();
        }

        if (addr.size() != Network::sin_size) {
            throw Network::SaLengthError(std::to_string(addr.size()),
                                         Network::sin_size,
                                         Network::sin_size);
        }
    }
}

auto Network::get_sin_pointer(const ByteString& addr) -> const sockaddr_in*
{
    validate_sin(addr);
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in*>(pointer);
}

// cppcheck-suppress constParameterReference
auto Network::get_sin_pointer(ByteString& addr) -> sockaddr_in*
{
    validate_sin(addr);
    void* pointer = addr.data();
    return static_cast<sockaddr_in*>(pointer);
}
