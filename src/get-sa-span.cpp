// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/get-sa-span.h"                // get_sa_span()
#include "network/get-sa-length.h"              // get_sa_length()
#include "network/get-sa-pointer.h"             // get_sa_pointer()
#include "network/socket-length-type.h"         // socket_length_type

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

#include <cstddef>      // std::byte
#include <span>         // std::span
#include <utility>      // std::pair

auto Network::get_sa_span(const std::span<std::byte>& span) ->
    std::pair<sockaddr*, socket_length_type>
{
    return {get_sa_pointer(span), get_sa_length(span)};
}

auto Network::get_sa_span(const std::span<const std::byte>& span) ->
    std::pair<const sockaddr*, socket_length_type>
{
    return {get_sa_pointer(span), get_sa_length(span)};
}
