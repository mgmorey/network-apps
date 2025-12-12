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

#include "network/inetsocket.hpp"               // InetSocket
#include "network/accept-socketcore.hpp"        // accept()
#include "network/acceptdata.hpp"               // AcceptData
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/charspan.hpp"                 // CharSpan
#include "network/close.hpp"                    // close()
#include "network/get-name.hpp"                 // get_name()
#include "network/handle-type.hpp"              // handle_type
#include "network/listen.hpp"                   // listen()
#include "network/namesymbol.hpp"               // NameSymbol
#include "network/open-handle.hpp"              // open()
#include "network/opensymbol.hpp"               // OpenSymbol
#include "network/oserror.hpp"                  // OsError
#include "network/read.hpp"                     // read()
#include "network/shutdown.hpp"                 // shutdown()
#include "network/socketcore.hpp"               // SocketCore
#include "network/socketdata.hpp"               // SocketData
#include "network/symbol.hpp"                   // Symbol
#include "network/write.hpp"                    // write()

#include <sys/types.h>      // ssize_t

#include <string_view>  // std::string_view
#include <utility>      // std::unreachable()

Network::InetSocket::InetSocket(const SocketData& t_sd) : m_sd(t_sd)
{
}

Network::InetSocket::~InetSocket() noexcept
{
    static_cast<void>(Network::close(core()));
}

Network::InetSocket::operator handle_type() const noexcept
{
    return core().handle();
}

auto Network::InetSocket::accept() const -> AcceptData
{
    return Network::accept(core());
}

auto Network::InetSocket::get_name(Symbol t_symbol) const -> ByteSpan
{
    auto& nm {m_sd.name(t_symbol)};

    if (!nm.empty()) {
        return nm;
    }

    const auto symbol {to_namesymbol(t_symbol)};
    nm = Network::get_name(core(), symbol);

    return nm;
}

auto Network::InetSocket::listen(int t_backlog) const -> OsError
{
    return Network::listen(core(), t_backlog);
}

auto Network::InetSocket::open(ByteSpan t_bs, OpenSymbol t_symbol) -> OsError
{
    if (const auto error {Network::open(core(), t_bs, t_symbol)}) {
        return error;
    }

    const auto symbol {to_symbol(t_symbol)};

    m_sd.name(symbol).assign(t_bs.begin(), t_bs.end());

    return {};
}

auto Network::InetSocket::read(CharSpan t_cs) const -> ssize_t
{
    return Network::read(core(), t_cs);
}

auto Network::InetSocket::shutdown(int t_how) const -> OsError
{
    return Network::shutdown(core(), t_how);
}

auto Network::InetSocket::write(std::string_view t_sv) const -> ssize_t
{
    return Network::write(core(), t_sv);
}

auto Network::InetSocket::core() const noexcept -> const SocketCore&
{
    return m_sd.core();
}

auto Network::InetSocket::to_namesymbol(Symbol symbol) noexcept -> NameSymbol
{
    switch (symbol) {
    case Symbol::getpeername:
        return NameSymbol::getpeername;
    case Symbol::getsockname:
        return NameSymbol::getsockname;
    default:
        std::unreachable();
    }
}

auto Network::InetSocket::to_symbol(OpenSymbol symbol) noexcept -> Symbol
{
    switch (symbol) {
    case OpenSymbol::bind:
        return Symbol::bind;
    case OpenSymbol::connect:
        return Symbol::connect;
    default:
        std::unreachable();
    }
}
