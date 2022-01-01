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

#include "network/get-sa-length.h"      // Bytes, get_sa_length(),
                                        // sock_len_type
#include "network/get-sa-pointer.h"     // get_sa_pointer(), sockaddr

auto Network::get_sa_length(const Bytes& addr,
                            sock_len_type length) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    const auto *const sa {get_sa_pointer(addr)};

    if (offsetof(sockaddr, sa_len) + sizeof sa->sa_len <= addr.size()) {
        if (sa->sa_len != 0) {
            length = sa->sa_len;
        }
    }
#else
    static_cast<void>(addr);
#endif
    return length;
}
