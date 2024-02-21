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

#include "network/get-sin6-addr.h"      // get_sin6_addr()
#include "network/assert.h"             // assert()
#include "network/bytestring.h"         // ByteString
#include "network/get-sa-family.h"      // get_sa_family()
#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/sin6-offsets.h"       // sin6_addr_offset

#ifdef WIN32
#include <winsock2.h>       // AF_INET6
#include <ws2tcpip.h>       // in6_addr
#else
#include <netinet/in.h>     // in6_addr
#include <sys/socket.h>     // AF_INET6
#endif

auto Network::get_sin6_addr(const ByteString& addr) -> in6_addr
{
    const auto* const sin6 {get_sin6_pointer(addr)};
    return sin6->sin6_addr;
}
