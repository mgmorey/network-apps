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

#include "network/get-sa-length.h"      // get_sa_length()
#include "network/bytestring.h"         // ByteString
#include "network/get-sa-pointer.h"     // get_sa_pointer()
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/sa-offsets.h"         // sa_len_offset
#include "network/sock-len-type.h"      // sock_len_type

auto Network::get_sa_length(const ByteString& addr,
                            sock_len_type length) noexcept ->
    Network::sock_len_type
{
    if (addr.empty()) {
        return length;
    }

#ifdef HAVE_SOCKADDR_SA_LEN
    const auto* const sa {get_sa_pointer(addr)};

    if (addr.size() < sa_len_offset + sizeof sa->sa_len) {
        return length;
    }

    if (sa->sa_len == 0) {
        return length;
    }

    return sa->sa_len;
#else
    return length;
#endif
}
