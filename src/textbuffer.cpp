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

#include "network/textbuffer.h"         // TextBuffer
#include "network/buffer.h"             // Buffer

Network::TextBuffer::TextBuffer(size_type t_size)
    : Buffer(t_size)
{
}

Network::TextBuffer::operator string_type() const
{
    const auto& str {this->get()};
    const auto last {str.find('\0')};
    return str.substr(0, last);
}
