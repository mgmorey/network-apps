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

#ifndef NETWORK_TEXTBUFFER_H
#define NETWORK_TEXTBUFFER_H

#include "network/buffer.h"     // Buffer

#include <string>       // std::string

namespace Network
{
    class TextBuffer : public Buffer<std::string>
    {
    public:
        using string_type = std::string;

        explicit TextBuffer(size_type t_size);

        TextBuffer(const TextBuffer&) noexcept = default;
        TextBuffer(TextBuffer&&) noexcept = default;
        ~TextBuffer() noexcept = default;
        auto operator=(const TextBuffer&) noexcept -> TextBuffer& = default;
        auto operator=(TextBuffer&&) noexcept -> TextBuffer& = default;

        // NOLINTNEXTLINE
        operator string_type() const;
    };
}

#endif
