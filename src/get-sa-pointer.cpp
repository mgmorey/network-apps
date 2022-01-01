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

#include "network/get-sa-pointer.h"     // Bytes, get_sa_pointer(),
                                        // sockaddr, sockaddr_in,
                                        // sockaddr_in6

auto Network::get_sa_pointer(const Bytes& addr) -> const sockaddr*
{
    const void* pointer = addr.data();
    return static_cast<const sockaddr*>(pointer);
}

auto Network::get_sa_pointer(Bytes& addr) -> sockaddr*
{
    void* pointer = addr.data();
    return static_cast<sockaddr*>(pointer);
}
