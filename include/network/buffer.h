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

namespace Network
{
    template <typename T>
    class Buffer
    {
    public:
        using size_type = std::size_t;
        using span_type = std::span<typename T::value_type>;
        using value_type = typename T::value_type;

        explicit Buffer(size_type t_size)
            : m_buffer(t_size, {})
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
            return static_cast<span_type>(m_buffer);
        }

        [[nodiscard]] auto get() noexcept -> T&
        {
            return m_buffer;
        }

        [[nodiscard]] auto get_view() const noexcept -> const T&
        {
            return m_buffer;
        }

        [[nodiscard]] auto data() noexcept -> value_type*
        {
            return m_buffer.data();
        }

        auto resize(size_type t_size) noexcept -> void
        {
            return m_buffer.resize(t_size);
        }

        [[nodiscard]] auto size() const noexcept -> size_type
        {
            return m_buffer.size();
        }

    private:
        T m_buffer;
    };
}

#endif
