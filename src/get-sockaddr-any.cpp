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

#include "network/get-sockaddr.h"       // Byte, Bytes,
                                        // get_sockaddr(),
                                        // sockaddr_storage,
                                        // sockaddr_un
#include "network/get-bytespan.h"       // get_bytespan(), std::size_t
#include "network/sizes.h"              // sockaddr_size_max

auto Network::get_sockaddr() -> Network::Bytes
{
    const auto data {static_cast<Byte>(0)};
    const auto size {sockaddr_size_max};
    return {size, data};
}
