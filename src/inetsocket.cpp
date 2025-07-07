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
#include "network/accept.hpp"                   // accept()
#include "network/acceptdata.hpp"               // AcceptData
#include "network/bytespan.hpp"                 // ByteSpan
#include "network/charspan.hpp"                 // CharSpan
#include "network/close.hpp"                    // close()
#include "network/get-name.hpp"                 // get_name()
#include "network/get-namehandler.hpp"          // get_namehandler()
#include "network/get-openhandler.hpp"          // get_openhandler()
#include "network/handle-null.hpp"              // handle_null
#include "network/listen.hpp"                   // listen()
#include "network/open-handle.hpp"              // open()
#include "network/oserror.hpp"                  // OsError
#include "network/read.hpp"                     // read()
#include "network/sharedruntime.hpp"            // SharedRuntime
#include "network/shutdown.hpp"                 // shutdown()
#include "network/socketdata.hpp"               // SocketData
#include "network/write.hpp"                    // write()

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

Network::InetSocket::operator handle_type() const
{
    return m_sd.handle();
}

auto Network::InetSocket::accept() const -> AcceptData
{
    return Network::accept(m_sd);
}

auto Network::InetSocket::listen(int t_backlog) const -> OsError
{
    return Network::listen(m_sd, t_backlog);
}

auto Network::InetSocket::name(bool t_is_sock) const -> ByteSpan
{
    const auto nh {get_namehandler(t_is_sock)};
    auto& nm {m_sd.cache(nh.symbol())};

    if (nm.empty()) {
        nm = Network::get_name(m_sd, nh);
    }

    return nm;
}

auto Network::InetSocket::open(ByteSpan t_bs, bool t_is_bind) -> OsError
{
    const auto oh {get_openhandler(t_is_bind)};

    if (const auto error {Network::open(m_sd, t_bs, oh)}) {
        return error;
    }

    auto& nm {m_sd.cache(oh.symbol())};
    nm.assign(t_bs.begin(), t_bs.end());
    return {};
}

auto Network::InetSocket::read(CharSpan t_cs) const -> ssize_t
{
    return Network::read(m_sd, t_cs);
}

auto Network::InetSocket::shutdown(int t_how) const -> OsError
{
    return Network::shutdown(m_sd, t_how);
}

auto Network::InetSocket::write(std::string_view t_sv) const -> ssize_t
{
    return Network::write(m_sd, t_sv);
}

auto Network::InetSocket::runtime() const noexcept -> SharedRuntime
{
    return m_sd.runtime();
}
