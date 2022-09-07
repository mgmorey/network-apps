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

#include "network/oserrorresult.h"      // OsErrorResult

#include <utility>      // std::move()

Network::OsErrorResult::OsErrorResult(os_error_type t_number,
                                      const std::string& t_string) noexcept :
    m_number(t_number),
    m_string(t_string)
{
}

Network::OsErrorResult::OsErrorResult(os_error_type t_number,
                                      std::string&& t_string) noexcept :
    m_number(t_number),
    m_string(std::move(t_string))
{
}

auto
Network::OsErrorResult::operator==(const OsErrorResult& t_result) const noexcept ->
    bool
{
    return (number() == t_result.number() &&
            string() == t_result.string());
}

auto
Network::OsErrorResult::operator!=(const OsErrorResult& t_result) const noexcept ->
    bool
{
    return (number() != t_result.number() ||
            string() != t_result.string());
}

Network::OsErrorResult::operator bool() const noexcept
{
    return m_number != 0 || !m_string.empty();
}

auto Network::OsErrorResult::number() const noexcept -> Network::os_error_type
{
    return m_number;
}

auto Network::OsErrorResult::string() const noexcept -> std::string
{
    return m_string;
}
