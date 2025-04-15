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

#ifndef NETWORK_GET_SA_SPAN_BUFFER_HPP
#define NETWORK_GET_SA_SPAN_BUFFER_HPP

#include "network/binarybuffer.hpp"             // BinaryBuffer
#include "network/socket-length-type.hpp"       // socket_length_type

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

#include <utility>      // std::pair

namespace Network
{
    extern auto get_sa_span(BinaryBuffer& buffer) ->
        std::pair<sockaddr*, socket_length_type&>;
}

#endif
