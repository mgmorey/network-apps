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

#include "network/commonsocket.hpp"     // CommonSocket
#include "network/accept.hpp"           // accept()
#include "network/acceptresult.hpp"     // AcceptResult
#include "network/close.hpp"            // close()
#include "network/get-name.hpp"         // get_name()
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type
#include "network/listen.hpp"           // listen()
#include "network/open-handle.hpp"      // open()
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/read.hpp"             // read()
#include "network/shutdown.hpp"         // shutdown()
#include "network/socketdata.hpp"       // SocketData
#include "network/symbol.hpp"           // Symbol
#include "network/write.hpp"            // write()

#include <cstddef>      // std::byte, std::size_t
#include <iostream>     // std::cerr, std::endl
#include <span>         // std::span

Network::CommonSocket::CommonSocket(const SocketData& t_sd) :
    m_sd(t_sd)
{
}

Network::CommonSocket::~CommonSocket() noexcept
{
    if (const auto result {Network::close(m_sd)}) {
        std::cerr << result.string()
                  << std::endl;
    }
}

Network::CommonSocket::operator bool() const noexcept
{
    return m_sd.handle() != handle_null;
}

Network::CommonSocket::operator handle_type() const noexcept
{
    return m_sd.handle();
}

auto Network::CommonSocket::accept() const -> AcceptResult

{
    return Network::accept(m_sd);
}

auto Network::CommonSocket::bind(std::span<const std::byte> t_bs) ->
    OsErrorResult
{
    return open(t_bs, true);
}

auto Network::CommonSocket::cache(Symbol symbol) const -> std::span<const std::byte>
{
    return m_cache[symbol];
}

auto Network::CommonSocket::connect(std::span<const std::byte> t_bs) ->
    OsErrorResult
{
    return open(t_bs, false);
}

auto Network::CommonSocket::is_verbose() const noexcept -> bool
{
    return m_sd.is_verbose();
}

auto Network::CommonSocket::listen(int t_backlog) const -> OsErrorResult
{
    return Network::listen(m_sd, t_backlog);
}

auto Network::CommonSocket::peername() const -> std::span<const std::byte>
{
    return get_name(false);
}

auto Network::CommonSocket::read(std::span<char> t_cs) const -> ssize_t
{
    return Network::read(m_sd, t_cs);
}

auto Network::CommonSocket::shutdown(int t_how) const -> OsErrorResult
{
    return Network::shutdown(m_sd, t_how);
}

auto Network::CommonSocket::sockname() const -> std::span<const std::byte>
{
    return get_name(true);
}

auto Network::CommonSocket::write(std::string_view t_sv) const -> ssize_t
{
    return Network::write(m_sd, t_sv);
}

auto Network::CommonSocket::get_name(bool t_is_sockname) const ->
    std::span<const std::byte>
{
    const auto api {t_is_sockname ? Symbol::sockname : Symbol::peername};
    auto& name {m_cache[api]};

    if (name.empty()) {
        name = Network::get_name(m_sd, t_is_sockname);
     }

    return name;
}

auto Network::CommonSocket::open(std::span<const std::byte> t_bs,
                                 bool t_is_bind) const -> OsErrorResult
{
    const auto api {t_is_bind ? Symbol::bind : Symbol::connect};

    if (const auto result {Network::open(m_sd, t_bs, t_is_bind)}) {
        return result;
    }

    m_cache[api].assign(t_bs.begin(), t_bs.end());
    return {};
}
