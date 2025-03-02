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

Network::BinaryBuffer::BinaryBuffer(size_type t_size)
    : Buffer(t_size)
{
}

auto Network::BinaryBuffer::size(size_type t_size) -> buffer_type&
{
    this->resize(t_size);
    return **this;
}
