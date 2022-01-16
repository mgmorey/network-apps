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

#ifndef NETWORK_RESULT_H
#define NETWORK_RESULT_H

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    template<typename T>
    struct Result
    {
        Result() = default;

        Result(T t_number, std::string t_string) :
            m_string(std::move(t_string)),
            m_number(t_number)
        {
        }

        operator bool() const  // NOLINT
        {
            return m_number != 0 || !m_string.empty();
        }

        [[nodiscard]] auto number() const -> T
        {
            return m_number;
        }

        [[nodiscard]] auto string() const -> std::string
        {
            return m_string;
        }

    private:
        std::string m_string {};
        T m_number {0};
    };
}

#endif
