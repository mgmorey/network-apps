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

#ifndef NETWORK_CREATE_SOCKET_HANDLE_HPP
#define NETWORK_CREATE_SOCKET_HANDLE_HPP

#include "network/family-type.hpp"      // family_type
#include "network/handle-type.hpp"      // handle_type
#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/uniquesocket.hpp"     // UniqueSocket

namespace Network
{
    extern auto create_socket(handle_type handle,
                              family_type family,
                              const SharedRuntime& sr) -> UniqueSocket;
    extern auto create_socket(handle_type handle,
                              family_type family,
                              bool is_verbose = false) -> UniqueSocket;
}

#endif
