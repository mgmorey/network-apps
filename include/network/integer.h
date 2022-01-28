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
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE
        constexpr Integer(T t_value = 0) noexcept :
            m_value(t_value)
        {
        }

        constexpr Integer(const Integer&) noexcept = default;
        constexpr Integer(Integer&&) noexcept = default;
        constexpr ~Integer() noexcept = default;
        constexpr auto operator=(const Integer&) noexcept -> Integer& = default;
        constexpr auto operator=(Integer&&) noexcept -> Integer& = default;

        constexpr auto operator=(T t_value) noexcept -> Integer&
        {
            m_value = t_value;
            return *this;
        }

        constexpr operator T() const noexcept  // NOLINT
        {
            return m_value;
        }

    private:
        T m_value {0};
    };
}

#endif
