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

#include "network/get-sin6-pointer.h"   // get_sin6_pointer()
#include "network/bytestring.h"         // ByteString

#ifdef WIN32
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#endif

auto Network::get_sin6_pointer(const ByteString& addr) noexcept ->
    const sockaddr_in6*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in6*>(pointer);
}

auto Network::get_sin6_pointer(ByteString& addr) noexcept -> sockaddr_in6*
{
    void* pointer = addr.data();
    return static_cast<sockaddr_in6*>(pointer);
}
