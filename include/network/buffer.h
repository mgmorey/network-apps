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

#ifndef NETWORK_BUFFER_H
#define NETWORK_BUFFER_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Buffer
    {
    public:
        explicit Buffer(std::string::size_type t_size = 0);
        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;
        operator std::string() const noexcept;  // NOLINT
        [[nodiscard]] auto data() noexcept -> char*;
        [[nodiscard]] auto size() const noexcept -> std::string::size_type;

    private:
        std::string m_value;
    };

    extern auto operator<<(std::ostream& os,
                           const Buffer& buffer) noexcept -> std::ostream&;
}

#endif
