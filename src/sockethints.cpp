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

#include "network/sockethints.h"        // SocketHints, operator<<(),
                                        // std::ostream

auto Network::SocketHints::flags() const noexcept -> SocketFlags
{
    return SocketFlags {m_flags};
}

auto Network::SocketHints::family() const noexcept -> SocketFamily
{
    return SocketFamily {m_family};
}

auto Network::SocketHints::socktype() const noexcept -> SocketType
{
    return SocketType {m_socktype};
}

auto Network::SocketHints::protocol() const noexcept -> SocketProtocol
{
    return SocketProtocol {m_family, m_protocol};
}
