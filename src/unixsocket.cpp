// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/unixsocket.h"                 // UnixSocket
#include "network/bytestring.h"                 // ByteString
#include "network/handle-type.h"                // handle_type
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket-family-type.h"         // socket_family_type
#include "network/to-path.h"                    // to_path()

#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl

Network::UnixSocket::UnixSocket(socket_family_type t_family,
                                handle_type t_handle,
                                bool t_is_verbose)
    : CommonSocket(t_family, t_handle, t_is_verbose)
{
}

Network::UnixSocket::~UnixSocket() noexcept
{
    state(SocketState::closing);
}

auto Network::UnixSocket::open(const ByteString& t_addr,
                               bool t_is_bind) -> OsErrorResult
{
    auto result {CommonSocket::open(t_addr, t_is_bind)};

    if (!result) {
        state(t_is_bind ? SocketState::bound : SocketState::connected);
    }

    return result;
}

auto Network::UnixSocket::remove(const PathnameView& t_path) const -> bool
{
    if (t_path.empty()) {
        return false;
    }

    if (is_verbose()) {
        std::cout << "Calling std::filesystem::remove("
                  << t_path
                  << ')'
                  << std::endl;
    }

    return std::filesystem::remove(t_path);
}

auto Network::UnixSocket::state(SocketState t_state) -> UnixSocket&
{
    const auto current {t_state};
    const auto previous {m_state};

    if (previous == current) {
        return *this;
    }

    if (current == SocketState::bound) {
        m_path = to_path(sockname());
    }
    else if (previous == SocketState::bound) {
        static_cast<void>(remove(m_path));
    }

    m_state = current;
    return *this;
}
