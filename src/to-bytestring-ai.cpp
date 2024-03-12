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

#include "network/to-bytestring-ai.h"           // to_bytestring()
#include "network/bytestring.h"                 // ByteString
#include "network/to-bytestring-auto.h"         // to_bytestring()
#include "network/to-sa-len.h"                  // to_sa_len()

#ifdef WIN32
#include <ws2tcpip.h>   // addrinfo
#else
#include <netdb.h>      // addrinfo
#endif

auto Network::to_bytestring(const addrinfo& ai) -> ByteString
{
    return to_bytestring(ai.ai_addr, to_sa_len(ai.ai_addrlen));
}
