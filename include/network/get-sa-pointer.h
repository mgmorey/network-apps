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

#ifndef NETWORK_GET_SA_POINTER_H
#define NETWORK_GET_SA_POINTER_H

#include "network/bytestring.h"         // ByteString
#include "network/os-features.h"        // WIN32

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

namespace Network
{
    extern auto get_sa_pointer(const ByteString& addr) noexcept -> const sockaddr*;
    extern auto get_sa_pointer(ByteString& addr) noexcept -> sockaddr*;
}

#endif
