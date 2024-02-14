// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_VALIDATE_SIN6_H
#define NETWORK_VALIDATE_SIN6_H

#include "network/sa-len-type.h"                // sa_len_type

#ifdef WIN32
#include <ws2tcpip.h>       // sockaddr_in6
#else
#include <netinet/in.h>     // sockaddr_in6
#endif

namespace Network
{
    extern auto validate(const sockaddr_in6 *sin6,
                         sa_len_type sin6_len = sizeof(sockaddr_in6)) -> void;
}

#endif
