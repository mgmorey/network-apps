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

#ifndef NETWORK_LIMITS_HPP
#define NETWORK_LIMITS_HPP

#include "network/name-length-limits.hpp"       // name_length_max,
                                                // name_length_min
#ifndef WIN32
#include "network/path-length-limits.hpp"       // path_length_max,
                                                // path_length_min
#endif
#include "network/sa-length-limits.hpp"         // sa_length_max,
                                                // sa_length_min
#include "network/sin-length-limits.hpp"        // sin_length_max,
                                                // sin_length_min
#include "network/sin6-length-limits.hpp"       // sin6_length_max,
                                                // sin6_length_min
#ifndef WIN32
#include "network/sun-length-limits.hpp"        // sun_length_max,
                                                // sun_length_min
#endif

#endif
