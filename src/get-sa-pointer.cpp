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

#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/validate-sa.h"                // validate()

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span

auto Network::get_sa_pointer(const std::span<std::byte>& span) -> sockaddr*
{
    void* pointer {span.data()};
    return static_cast<sockaddr*>(pointer);
}

auto Network::get_sa_pointer(const std::span<const std::byte>& span) ->
    const sockaddr*
{
    const void* pointer {span.data()};
    return validate(static_cast<const sockaddr*>(pointer), span.size());
}
