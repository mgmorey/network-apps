// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/write-string.h"       // write_string()
#include "network/socket.h"             // Socket
#include "network/write.h"              // write()

#include <sys/types.h>          // ssize_t

#include <string_view>  // std::string_view
#include <utility>      // std::move()

auto Network::write(Socket sock, std::string_view str) -> ssize_t
{
    return write(std::move(sock), str.data(), str.size());
}
