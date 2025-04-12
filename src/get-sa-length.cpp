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

#include "network/get-sa-length.hpp"            // get_sa_length()
#include "network/socket-length-type.hpp"       // socket_length_type
#include "network/to-socket-length.hpp"         // to_socket_length()

#include <cstddef>      // std::byte
#include <span>         // std::span

auto Network::get_sa_length(std::span<std::byte> bs) ->
    socket_length_type
{
    return to_socket_length(bs.size());
}

auto Network::get_sa_length(std::span<const std::byte> bs) ->
    socket_length_type
{
    return to_socket_length(bs.size());
}
