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

#ifndef NETWORK_BINARYBUFFER_HPP
#define NETWORK_BINARYBUFFER_HPP

#include "network/buffer.hpp"                   // Buffer
#include "network/bytestring.hpp"               // ByteString
#include "network/socket-length-type.hpp"       // socket_length_type

namespace Network
{
    class BinaryBuffer : public Buffer<ByteString>
    {
    public:
        BinaryBuffer();

        BinaryBuffer(const BinaryBuffer&) noexcept = default;
        BinaryBuffer(BinaryBuffer&&) noexcept = default;
        ~BinaryBuffer() noexcept = default;
        auto operator=(const BinaryBuffer&) noexcept -> BinaryBuffer& = default;
        auto operator=(BinaryBuffer&&) noexcept -> BinaryBuffer& = default;
        auto operator*() noexcept -> buffer_type&;
        [[nodiscard]] auto length() noexcept -> socket_length_type&;

    private:
        socket_length_type m_length {0};
    };
}

#endif
