// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/socketlimits.h"               // SocketLimits
#include "network/family-type.h"                // family_type
#include "network/limits.h"                     // sa_length_max,
                                                // sa_length_min,
                                                // sin_length_max,
                                                // sin_length_min,
                                                // sin6_length_max,
                                                // sin6_length_min,
                                                // sun_length_max,
                                                // sun_length_min

#ifdef WIN32
#include <winsock2.h>       // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#else
#include <sys/socket.h>     // AF_INET, AF_INET6, AF_UNIX, AF_UNSPEC
#endif

#include <cstddef>      // std::size_t
#include <utility>      // std::pair

Network::SocketLimits::SocketLimits(family_type t_family) noexcept
    : m_family(t_family)
{
}

auto Network::SocketLimits::limits() const noexcept ->
    std::pair<std::size_t, std::size_t>
{
    return {min(), max()};
}

auto Network::SocketLimits::max() const noexcept -> std::size_t
{
    switch (m_family) {
    case AF_UNSPEC:
        return sa_length_max;
#ifndef WIN32
    case AF_UNIX:
        return sun_length_max;
#endif
    case AF_INET:
        return sin_length_max;
    case AF_INET6:
        return sin6_length_max;
    default:
        return sa_length_max;
    }
}

auto Network::SocketLimits::min() const noexcept -> std::size_t
{
    switch (m_family) {
    case AF_UNSPEC:
        return sa_length_min;
#ifndef WIN32
    case AF_UNIX:
        return sun_length_min;
#endif
    case AF_INET:
        return sin_length_min;
    case AF_INET6:
        return sin6_length_min;
    default:
        return sa_length_min;
    }
}
