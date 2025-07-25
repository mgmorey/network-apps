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

#include "network/get-sin6-pointer.hpp"         // get_sin6_pointer()
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/validate-sin6.hpp"            // validate()

#ifdef _WIN32
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#endif

auto Network::get_sin6_pointer(ByteSpan bs) -> const sockaddr_in6*
{
    const void* pointer {bs.data()};
    return validate(static_cast<const sockaddr_in6*>(pointer));
}
