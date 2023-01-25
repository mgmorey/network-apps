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

#ifndef NETWORK_SOCK_LEN_TYPE_H
#define NETWORK_SOCK_LEN_TYPE_H

#ifndef WIN32
#include <sys/socket.h>     // socklen_t
#endif

namespace Network
{
#ifdef WIN32
    using sock_len_type = int;
#else
    using sock_len_type = socklen_t;
#endif
}

#endif
