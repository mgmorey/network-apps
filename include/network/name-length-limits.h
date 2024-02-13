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

#ifndef NETWORK_NAME_LENGTH_LIMITS_H
#define NETWORK_NAME_LENGTH_LIMITS_H

#ifdef WIN32
#include <ws2tcpip.h>   // NI_MAXHOST
#else
#include <netdb.h>      // NI_MAXHOST
#endif

#ifndef NI_MAXHOST
#define	NI_MAXHOST	(1025)	// NOLINT
#endif

namespace Network
{
    static constexpr auto name_length_max {NI_MAXHOST};
    static constexpr auto name_length_min {0};
}

#endif
