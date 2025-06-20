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

#include "network/inetsocket.hpp"       // InetSocket
#include "network/accept.hpp"           // accept()
#include "network/acceptresult.hpp"     // AcceptResult
#include "network/close.hpp"            // close()
#include "network/get-name.hpp"         // get_name()
#include "network/get-namehandler.hpp"  // get_namehandler()
#include "network/get-openhandler.hpp"  // get_openhandler()
#include "network/handle-null.hpp"      // handle_null
#include "network/listen.hpp"           // listen()
#include "network/open-handle.hpp"      // open()
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/read.hpp"             // read()
#include "network/shutdown.hpp"         // shutdown()
#include "network/socketdata.hpp"       // SocketData
#include "network/symbol.hpp"           // Symbol
#include "network/write.hpp"            // write()

#include <cstddef>      // std::byte, std::size_t
#include <span>         // std::span
#include <string>       // std::string, std::to_string()
#include <tuple>        // std::get()

Network::InetSocket::InetSocket(const SocketData& t_sd) : m_sd(t_sd)
{
}

Network::InetSocket::~InetSocket() noexcept
{
    static_cast<void>(Network::close(m_sd));
}

Network::InetSocket::operator bool() const noexcept
{
    return m_sd.handle() != handle_null;
}

Network::InetSocket::operator std::string() const
{
    return std::to_string(m_sd.handle());
}

auto Network::InetSocket::accept() const -> AcceptResult

{
    return Network::accept(m_sd);
}

auto Network::InetSocket::bind(std::span<const std::byte> t_bs) ->
    OsErrorResult
{
    return open(t_bs, true);
}

auto Network::InetSocket::cache(Symbol symbol) const -> std::span<const std::byte>
{
    return m_cache[symbol];
}

auto Network::InetSocket::connect(std::span<const std::byte> t_bs) ->
    OsErrorResult
{
    return open(t_bs, false);
}

auto Network::InetSocket::is_verbose() const noexcept -> bool
{
    return m_sd.runtime()->is_verbose();
}

auto Network::InetSocket::listen(int t_backlog) const -> OsErrorResult
{
    return Network::listen(m_sd, t_backlog);
}

auto Network::InetSocket::peername() const -> std::span<const std::byte>
{
    return get_name(false);
}

auto Network::InetSocket::read(std::span<char> t_cs) const -> ssize_t
{
    return Network::read(m_sd, t_cs);
}

auto Network::InetSocket::shutdown(int t_how) const -> OsErrorResult
{
    return Network::shutdown(m_sd, t_how);
}

auto Network::InetSocket::sockname() const -> std::span<const std::byte>
{
    return get_name(true);
}

auto Network::InetSocket::write(std::string_view t_sv) const -> ssize_t
{
    return Network::write(m_sd, t_sv);
}

auto Network::InetSocket::get_name(bool t_is_sockname) const ->
    std::span<const std::byte>
{
    const auto handler {get_namehandler(t_is_sockname)};
    auto& name {m_cache[std::get<2>(handler)]};

    if (name.empty()) {
        name = Network::get_name(m_sd, handler);
    }

    return name;
}

auto Network::InetSocket::open(std::span<const std::byte> t_bs,
                               bool t_is_bind) const -> OsErrorResult
{
    const auto handler {get_openhandler(t_is_bind)};

    if (const auto os_error {Network::open(m_sd, t_bs, handler)}) {
        return os_error;
    }

    auto& name {m_cache[std::get<2>(handler)]};
    name.assign(t_bs.begin(), t_bs.end());
    return {};
}
