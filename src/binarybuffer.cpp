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

#include "network/binarybuffer.h"       // BinaryBuffer
#include "network/buffer.h"             // Buffer
#include "network/sa-length-limits.h"   // sa_length_max
#include "network/to-size.h"            // to_size()

Network::BinaryBuffer::BinaryBuffer() :
    Buffer(sa_length_max),
    m_length(sa_length_max)
{
}

auto Network::BinaryBuffer::length() noexcept -> socket_length_type&
{
    return m_length;
}

auto Network::BinaryBuffer::value() noexcept -> Buffer::buffer_type&
{
    resize(to_size(m_length));
    return Buffer::value();
}
