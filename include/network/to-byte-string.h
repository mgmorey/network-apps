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

#ifndef NETWORK_TO_BYTE_STRING_H
#define NETWORK_TO_BYTE_STRING_H

#include "network/bytespan.h"           // ByteSpan
#include "network/bytestring.h"         // ByteString
#include "network/optionalpathname.h"   // OptionalPathname
#ifndef _WIN32
#include "network/sun-sizes.h"          // sun_size
#endif

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#include <sys/un.h>         // sockaddr_un
#endif

#include <cstddef>      // std::size_t

namespace Network
{
    extern auto to_byte_string(const ByteSpan& span) -> ByteString;
    extern auto to_byte_string(const sockaddr* sa,
                               std::size_t size) -> ByteString;
#ifndef _WIN32
    extern auto to_byte_string(const sockaddr_un* sun,
                               std::size_t size = sun_size) -> ByteString;
    extern auto to_byte_string(const OptionalPathname& pathname = {}) -> ByteString;
#endif
}

#endif
