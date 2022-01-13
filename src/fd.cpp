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

#include "network/fd.h"                 // Fd, fd_type, operator<<(),
                                        // std::ostream

Network::Fd::Fd(fd_type t_fd) :
    m_value(t_fd)
{
}

Network::Fd::operator fd_type() const
{
    return m_value;
}

Network::Fd::operator bool() const
{
    return m_value != fd_null;
}

auto Network::operator<<(std::ostream& os, Fd fd) -> std::ostream&
{
    return os << fd.m_value;
}
