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

#include "network/read-string.h"        // Socket, descriptor_type,
                                        // read_string(), ssize_t,
                                        // std::size_t
#include "network/buffer.h"             // Buffer
#include "network/to-integer.h"         // to_integer()

#ifdef WIN32
#include <winsock2.h>       // ::recv()
#else
#include <unistd.h>         // ::read()
#endif

auto Network::read_string(std::size_t size, const Socket& sock) ->
    std::pair<std::string, ssize_t>
{
    Buffer buffer {size};
    const descriptor_type handle {sock};
#ifdef WIN32
    const auto code {
        ::recv(handle, buffer.data(), to_integer(buffer.size()), 0)}
    ;
#else
    const auto code {
        ::read(handle, buffer.data(), buffer.size())
    };
#endif
    return {buffer, code};
}
