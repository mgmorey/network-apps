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

#ifndef NETWORK_INTEGER_H
#define NETWORK_INTEGER_H

namespace Network
{
    template<typename T>
    class Integer
    {
    public:
        Integer() = default;
        Integer(const Integer& t_integer) = default;
        Integer(Integer&& t_integer) noexcept = default;

        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE
        Integer(T t_value) :
            m_value(t_value)
        {
        }

        ~Integer() = default;
        auto operator=(const Integer& t_integer) -> Integer& = default;
        auto operator=(Integer&& t_integer) noexcept -> Integer& = default;

        auto operator=(T t_value) -> Integer&
        {
            m_value = t_value;
            return *this;
        }

        operator T() const  /// NOLINT
        {
            return m_value;
        }

    private:
        T m_value {0};
    };
}

#endif
