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

#ifndef NETWORK_LIMITS_H
#define NETWORK_LIMITS_H

#include "network/name-length-limits.h"         // name_length_max,
                                                // name_length_min
#include "network/os-error-limits.h"            // os_error_max,
                                                // os_error_min
#ifndef WIN32
#include "network/path-length-limits.h"         // path_length_max,
                                                // path_length_min
#endif
#include "network/sa-len-limits.h"              // sa_len_max,
                                                // sa_len_min
#include "network/socket-length-limits.h"       // socket_length_max,
                                                // socket_length_min
#ifndef WIN32
#include "network/sun-len-limits.h"             // sun_len_max,
                                                // sun_len_min
#endif

#endif
