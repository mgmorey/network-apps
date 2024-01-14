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

#include "network/get-length.h"         // get_length()
#include "network/bytestring.h"         // ByteString
#include "network/os-features.h"        // HAVE_SOCKADDR_SA_LEN
#include "network/sock-len-type.h"      // sock_len_type,

#ifdef HAVE_SOCKADDR_SA_LEN
#include "network/get-sa-length.h"      // get_sa_length()
#else
#include "network/to-sock-len.h"        // to_sock_len()
#endif

auto Network::get_length(const ByteString& addr) -> Network::sock_len_type
{
#ifdef HAVE_SOCKADDR_SA_LEN
    return get_sa_length(addr);
#else
    return to_sock_len(addr.length());
#endif

}
