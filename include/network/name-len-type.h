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

#ifndef NETWORK_NAME_LEN_TYPE_H
#define NETWORK_NAME_LEN_TYPE_H

#ifdef WIN32
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#endif

#ifndef NI_MAXHOST
#define	NI_MAXHOST	(1025)	// NOLINT
#endif

#include <cstddef>      // std::size_t

namespace Network
{
#ifdef WIN32
    using name_len_type = int;
#else
    using name_len_type = std::size_t;
#endif
    static constexpr auto name_len_max {NI_MAXHOST};
    static constexpr auto name_len_min {0};
}

#endif
