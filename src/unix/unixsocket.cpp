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
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/socketdata.hpp"       // SocketData
#include "network/to-path.hpp"          // to_path()

#include <cstddef>      // std::byte
#include <filesystem>   // std::filesystem
#include <iostream>     // std::cout, std::endl
#include <span>         // std::span

Network::UnixSocket::UnixSocket(const SocketData& t_sd) :
    CommonSocket(t_sd)
{
}

Network::UnixSocket::~UnixSocket() noexcept
{
    state(SocketState::closing);
}

auto Network::UnixSocket::bind(std::span<const std::byte> t_bs) -> OsErrorResult
{
    if (auto result {CommonSocket::bind(t_bs)}) {
        return result;
    }

    state(SocketState::bound);
    return {};
}

auto Network::UnixSocket::connect(std::span<const std::byte> t_bs) ->
    OsErrorResult
{
    if (auto result {CommonSocket::connect(t_bs)}) {
        return result;
    }

    state(SocketState::connected);
    return {};
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
    const auto& current {t_state};
    auto& previous {m_state};

    if (previous == current) {
        return *this;
    }

    if (current == SocketState::bound) {
        m_path = to_path(sockname());
    }
    else if (previous == SocketState::bound) {
        static_cast<void>(remove(m_path));
    }

    previous = current;
    return *this;
}
