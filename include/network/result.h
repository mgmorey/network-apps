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

#include <string>       // std::string

namespace Network
{
    template<typename T>
    struct Result
    {
        Result() noexcept = default;

        Result(T t_number, const std::string& t_string) noexcept :
            m_string(t_string),
            m_number(t_number)
        {
        }

        auto operator==(const Result<T>& t_result) const noexcept -> bool
        {
            return (number() == t_result.number() &&
                    string() == t_result.string());
        }

        auto operator!=(const Result<T>& t_result) const noexcept -> bool
        {
            return (number() != t_result.number() ||
                    string() != t_result.string());
        }

        operator bool() const noexcept  // NOLINT
        {
            return m_number != 0 || !m_string.empty();
        }

        [[nodiscard]] auto number() const noexcept -> T
        {
            return m_number;
        }

        [[nodiscard]] auto string() const noexcept -> std::string
        {
            return m_string;
        }

    private:
        std::string m_string {};
        T m_number {0};
    };
}

#endif
