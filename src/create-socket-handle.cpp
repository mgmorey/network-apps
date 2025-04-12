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

#include "network/create-socket-handle.hpp"     // create_socket()
#include "network/create-socket.hpp"            // create_socket()
#include "network/family-type.hpp"              // family_type
#include "network/handle-type.hpp"              // handle_type
#include "network/socketdata.hpp"               // SocketData

auto Network::create_socket(handle_type handle,
                            family_type family,
                            bool is_verbose) -> UniqueSocket
{
    return create_socket(SocketData {handle, family, is_verbose});
}
