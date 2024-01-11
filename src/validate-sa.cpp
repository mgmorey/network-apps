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

#include "network/validate-sa.h"        // validate()
#include "network/logicerror.h"         // LogicError
#include "network/sa-len-type.h"        // sa_len_type
#include "network/sa-offsets.h"         // sa_data_offset

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, sockaddr
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, sockaddr
#endif

auto Network::validate(const sockaddr* sa,
                       sa_len_type sa_len) -> void
{
    if (sa_len < sa_data_offset ||
        (sa->sa_family != AF_INET &&
         sa->sa_family != AF_INET6)) {
        throw LogicError("Invalid IP domain socket address");
    }
}
