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
        using buffer_type = T;
        using size_type = std::size_t;
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

        [[nodiscard]] auto data() noexcept -> value_type*
        {
            return m_buffer.data();
        }

        [[nodiscard]] auto data() const noexcept -> const value_type*
        {
            return m_buffer.data();
        }

        [[nodiscard]] auto get() noexcept -> buffer_type&
        {
            return m_buffer;
        }

        [[nodiscard]] auto get() const noexcept -> const buffer_type&
        {
            return m_buffer;
        }

        auto resize(size_type t_size) noexcept -> void
        {
            return m_buffer.resize(t_size);
        }

        [[nodiscard]] auto size() const noexcept -> size_type
        {
            return m_buffer.size();
        }

        [[nodiscard]] auto span() noexcept -> std::span<value_type>
        {
            return std::span {m_buffer.data(), m_buffer.size()};
        }

        [[nodiscard]] auto span() const noexcept -> std::span<const value_type>
        {
            return std::span {m_buffer.data(), m_buffer.size()};
        }

    private:
        buffer_type m_buffer;
    };
}

#endif
