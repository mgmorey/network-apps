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

#ifndef NETWORK_SOCKETPAIR_TYPE_H
#define NETWORK_SOCKETPAIR_TYPE_H

#include "network/handle-type.h"                // handle_type
#include "network/socketpair-size.h"            // socketpair_size

#include <array>        // std::array

namespace Network
{
#ifndef WIN32
    using socketpair_type = std::array<handle_type, socketpair_size>;
#endif
}

#endif
