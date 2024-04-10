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

#include "network/to-string-vector-byte.h"      // to_string()
#include "network/to-string-vector-char.h"      // to_string()

#include <cstddef>      // std::size_t
#include <string>       // std::basic_string
#include <vector>       // std::vector

namespace Network
{
    template<typename T>
    class Buffer
    {
    public:
        using size_type = std::size_t;
        using value_type = std::vector<T>;

        explicit Buffer(size_type t_size)
            : m_value(t_size, {})
        {
        }

        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;

        operator value_type() const noexcept  // NOLINT
        {
            return m_value;
        }

        operator std::basic_string<T>() const  // NOLINT
        {
            return to_string(m_value);
        }

        [[nodiscard]] auto data() const noexcept -> const T*
        {
            return m_value.data();
        }

        [[nodiscard]] auto data() noexcept -> T*
        {
            return m_value.data();
        }

        auto resize(size_type t_size) -> void
        {
            return m_value.resize(t_size);
        }

        [[nodiscard]] auto size() const noexcept -> size_type
        {
            return m_value.size();
        }

    private:
        value_type m_value;
    };
}

#endif
