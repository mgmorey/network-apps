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

#include "network/get-length.h"         // ByteString, get_length(),
                                        // sock_len_type
#include "network/get-sa-length.h"      // get_sa_length()

auto Network::get_length(const ByteString& addr) noexcept ->
    Network::sock_len_type
{
    const auto len {static_cast<sock_len_type>(addr.length())};
    return get_sa_length(addr, len);
}
