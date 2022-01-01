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

#include "network/get-sin-pointer.h"    // Bytes,
                                        // get_sin_pointer(),
                                        // sockaddr_in

auto Network::get_sin_pointer(const Bytes& addr) -> const sockaddr_in*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr_in*>(pointer);
}

auto Network::get_sin_pointer(Bytes& addr) -> sockaddr_in*
{
    void* pointer = addr.data();
    return static_cast<sockaddr_in*>(pointer);
}
