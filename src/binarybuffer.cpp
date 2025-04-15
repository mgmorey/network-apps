// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/binarybuffer.hpp"             // BinaryBuffer
#include "network/buffer.hpp"                   // Buffer
#include "network/sa-length-limits.hpp"         // sa_length_max
#include "network/to-size.hpp"                  // to_size()

#ifdef WIN32
#include <winsock2.h>       // sockaddr
#else
#include <sys/socket.h>     // sockaddr
#endif

Network::BinaryBuffer::BinaryBuffer() :
    Buffer(sa_length_max),
    m_length(sa_length_max)
{
}

auto Network::BinaryBuffer::operator*() noexcept -> Buffer::buffer_type&
{
    resize(to_size(m_length));
    return Buffer::value();
}

auto Network::BinaryBuffer::span() -> span_type
{
    void* pointer {data()};
    auto& length {m_length};
    return {static_cast<sockaddr*>(pointer), length};
}
