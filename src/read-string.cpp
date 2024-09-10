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

#include "network/read-string.h"        // read_string()
#include "network/buffer.h"             // Buffer
#include "network/readresult.h"         // ReadResult
#include "network/socket.h"             // Socket

#include <cstddef>      // std::size_t
#include <string>       // std::string

auto Network::read_string(Socket& sock, std::size_t size) -> ReadResult
{
    Buffer<char> buffer {size};
    const auto result {sock.read(buffer.data(), buffer.size())};
    return {std::string {buffer}, result};
}
