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

#ifndef NETWORK_SERVICE_LENGTH_LIMITS_H
#define NETWORK_SERVICE_LENGTH_LIMITS_H

#ifdef WIN32
#include <ws2tcpip.h>   // NI_MAXSERV
#else
#include <netdb.h>      // NI_MAXSERV
#endif

#ifndef NI_MAXSERV
#define	NI_MAXSERV	(32)	// NOLINT
#endif

namespace Network
{
    static constexpr auto service_length_max {NI_MAXSERV - 1};
    static constexpr auto service_length_min {0};
}

#endif
