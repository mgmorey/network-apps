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

#ifndef NETWORK_TO_BYTESPAN_SA_H
#define NETWORK_TO_BYTESPAN_SA_H

#include "network/bytespan.h"           // ByteSpan
#include "network/sa-len-limits.h"      // sa_len_max
#include "network/sa-len-type.h"        // sa_len_type

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

namespace Network
{
    extern auto to_bytespan(const sockaddr* sa,
                            sa_len_type sa_len = sa_len_max) noexcept -> ByteSpan;
}

#endif
