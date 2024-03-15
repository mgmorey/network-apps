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

#include "network/resize-bs.h"          // resize()
#include "network/resize-str.h"         // resize()

#include <string>       // std::basic_string

namespace Network
{
    template<typename T>
    class Buffer
    {
    public:
        explicit Buffer(typename std::basic_string<T>::size_type t_size) :
            m_value(t_size, {})
        {
        }

        Buffer(const Buffer&) noexcept = default;
        Buffer(Buffer&&) noexcept = default;
        ~Buffer() noexcept = default;
        auto operator=(const Buffer&) noexcept -> Buffer& = default;
        auto operator=(Buffer&&) noexcept -> Buffer& = default;

        operator std::basic_string<T>() const // NOLINT
        {
            return Network::resize(m_value);
        }

        [[nodiscard]] auto data() noexcept -> T*
        {
            return m_value.data();
        }

        auto resize(typename std::basic_string<T>::size_type t_size) -> void
        {
            return m_value.resize(t_size);
        }

        [[nodiscard]] auto size() const noexcept ->
            typename std::basic_string<T>::size_type
        {
            return m_value.size();
        }

    private:
        std::basic_string<T> m_value;
    };
}

#endif
