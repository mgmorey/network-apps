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

#include "network/get-sa-pointer.hpp"   // get_sa_pointer()
#include "network/bytespan.hpp"         // ByteSpan
#include "network/validate-sa.hpp"      // validate()

#ifdef _WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

auto Network::get_sa_pointer(ByteSpan bs) -> const sockaddr*
{
    const void* pointer {bs.data()};
    return validate(static_cast<const sockaddr*>(pointer), bs.size());
}
