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

#include "network/get-sa-family.h"              // get_sa_family()
#include "network/bytestring.h"                 // ByteString
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/socket-family-type.h"         // socket_family_type

auto Network::get_sa_family(const ByteString& addr) -> socket_family_type
{
    const auto* const sa {get_sa_pointer(addr)};
    return static_cast<socket_family_type>(sa->sa_family);
}
