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

#include "network/compact.h"            // compact()

#include <string>       // std::basic_string

namespace Network
{
    template<typename T>
    class Buffer
    {
    public:
        using value_type = std::basic_string<T>;

        explicit Buffer(typename value_type::size_type t_size) :
            m_value(t_size, {})
        {
        }

        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;

        operator value_type() const // NOLINT
        {
            return m_value;
        }

        [[nodiscard]] auto compact() -> value_type
        {
            return Network::compact(m_value);
        }

        [[nodiscard]] auto data() noexcept -> T*
        {
            return m_value.data();
        }

        auto resize(typename value_type::size_type t_size) -> void
        {
            return m_value.resize(t_size);
        }

        [[nodiscard]] auto size() const noexcept ->
            typename value_type::size_type
        {
            return m_value.size();
        }

    private:
        value_type m_value;
    };
}

#endif
