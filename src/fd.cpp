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
#include "network/close.h"              // close()
#include "network/get-socket.h"         // get_socket()
#include "network/string-null.h"        // string_null

#include <string>       // std::to_string

Network::Fd::Fd(socket_family_type t_family,
                socket_type_type t_socktype,
                socket_protocol_type t_protocol,
                socket_flags_type t_flags,
                bool t_verbose) :
    Fd(SocketHints {t_flags, t_family, t_socktype, t_protocol}, t_verbose)
{
}

Network::Fd::Fd(const SocketHints& t_hints, bool t_verbose) :
    Fd(get_socket(t_hints, t_verbose))
{
}

Network::Fd::Fd(fd_type t_fd, bool t_verbose) noexcept :
    m_fd(t_fd),
    m_verbose(t_verbose)
{
}

auto Network::Fd::operator=(const SocketHints& t_hints) noexcept -> Network::Fd&
{
    m_fd = Network::close(m_fd);
    m_fd = static_cast<fd_type>(get_socket(t_hints, m_verbose));
    return *this;
}

Network::Fd::operator bool() const noexcept
{
    return m_fd != fd_null;
}

Network::Fd::operator fd_type() const noexcept
{
    return m_fd;
}

Network::Fd::operator std::string() const
{
    if (m_fd == fd_null) {
        return string_null;
    }

    return std::to_string(m_fd);
}

auto Network::operator<<(std::ostream& os,
                         const Fd& fd) noexcept -> std::ostream&
{
    return os << static_cast<std::string>(fd);
}
