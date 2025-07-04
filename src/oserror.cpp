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

#include "network/oserror.hpp"          // OsError
#include "network/os-error-type.hpp"    // os_error_type

#include <string>       // std::string
#include <string_view>  // std::string_view

Network::OsError::OsError(os_error_type t_number,
                          std::string_view t_string) :
    m_number(t_number),
    m_string(t_string)
{
}

Network::OsError::operator bool() const noexcept
{
    return m_number != 0 || !m_string.empty();
}

auto Network::OsError::number() const noexcept -> os_error_type
{
    return m_number;
}

auto Network::OsError::string() const -> const std::string&
{
    return m_string;
}
