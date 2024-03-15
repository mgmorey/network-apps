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

#include "network/resize-str.h"         // resize()

#include <ostream>      // std::ostream

namespace Network
{
    template<typename T>
    class Buffer
    {
    public:
        explicit Buffer(typename T::size_type t_size = 0) :
            m_value(t_size, '\0')
        {
        }
        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;
        operator T() const // NOLINT
        {
            return resize(m_value);
        }
        [[nodiscard]] auto data() noexcept -> char*
        {
            return m_value.data();
        }
        [[nodiscard]] auto size() const noexcept -> typename T::size_type
        {
            return m_value.size();
        }

    private:
        T m_value;
    };
}

#endif
