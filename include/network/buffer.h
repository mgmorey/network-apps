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

#include <cstddef>      // std::size_t
#include <span>         // std::span
#include <ranges>       // std::ranges
#include <string>       // std::string

namespace Network
{
    template <typename T, typename V>
    class Buffer
    {
    public:
        using size_type = std::size_t;
        using span_type = std::span<T>;
        using string_type = std::string;

        explicit Buffer(size_type t_size)
            : m_value(t_size, {})
        {
        }

        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;

        // NOLINTNEXTLINE
        operator span_type() noexcept
        {
            return span_type(m_value);
        }

        // NOLINTNEXTLINE
        operator V()& noexcept
        {
            return m_value;
        }

        [[nodiscard]] auto data() noexcept -> T*
        {
            return m_value.data();
        }

        [[nodiscard]] auto size() const noexcept -> size_type
        {
            return m_value.size();
        }

        [[nodiscard]] auto size(size_type t_size) -> V&
        {
            m_value.resize(t_size);
            return m_value;
        }

        [[nodiscard]] auto to_string() const -> string_type
        {
            return {m_value.begin(), std::ranges::find(m_value, '\0')};
        }

    private:
        V m_value;
    };
}

#endif
