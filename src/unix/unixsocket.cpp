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

#include "network/unixsocket.hpp"       // UnixSocket
#include "network/bytespan.hpp"         // ByteSpan
#include "network/opensymbol.hpp"       // OpenSymbol
#include "network/oserror.hpp"          // OsError
#include "network/socketdata.hpp"       // SocketData
#include "network/to-path.hpp"          // to_path()

#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl

Network::UnixSocket::UnixSocket(const SocketData& t_sd) : InetSocket(t_sd)
{
}

Network::UnixSocket::~UnixSocket() noexcept
{
    state(SocketState::closing);
}

auto Network::UnixSocket::open(ByteSpan t_bs, OpenSymbol t_symbol) -> OsError
{
    if (auto error {InetSocket::open(t_bs, t_symbol)}) {
        return error;
    }

    switch (t_symbol) {
    case OpenSymbol::bind:
        state(SocketState::bound);
        break;
    case OpenSymbol::connect:
        state(SocketState::connected);
        break;
    }

    return {};
}

auto Network::UnixSocket::remove(const PathnameView& t_path) const -> bool
{
    if (t_path.empty()) {
        return false;
    }

    if (core().runtime()->is_verbose()) {
        std::cout << "Calling std::filesystem::remove("
                  << t_path
                  << ')'
                  << std::endl;
    }

    return std::filesystem::remove(t_path);
}

auto Network::UnixSocket::state(SocketState t_state) -> UnixSocket&
{
    const auto& current {t_state};
    auto& previous {m_state};

    if (previous == current) {
        return *this;
    }

    if (current == SocketState::bound) {
        m_path = to_path(get_sockname());
    }
    else if (previous == SocketState::bound) {
        static_cast<void>(remove(m_path));
    }

    previous = current;
    return *this;
}
