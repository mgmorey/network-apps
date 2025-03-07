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

#include "network/commonsocket.h"               // CommonSocket
#include "network/accept.h"                     // accept()
#include "network/acceptresult.h"               // AcceptResult
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-name.h"                   // get_name()
#include "network/handle-null.h"                // handle_null
#include "network/listen.h"                     // listen()
#include "network/open-handle.h"                // open()
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/read.h"                       // read()
#include "network/shutdown.h"                   // shutdown()
#include "network/socketdata.h"                 // SocketData
#include "network/string-null.h"                // string_null
#include "network/write.h"                      // write()

#include <cstddef>      // std::byte, std::size_t
#include <iostream>     // std::cerr, std::endl
#include <span>         // std::span
#include <string>       // std::string, std::to_string()

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

Network::CommonSocket::operator std::string() const
{
    if (m_sd.handle() == handle_null) {
        return string_null;
    }

    return std::to_string(m_sd.handle());
}

auto Network::CommonSocket::is_verbose() const noexcept -> bool
{
    return m_sd.is_verbose();
}

auto Network::CommonSocket::accept() const -> AcceptResult

{
    return Network::accept(m_sd);
}

auto Network::CommonSocket::bind(std::span<const std::byte> t_bs) -> OsErrorResult
{
    return open(t_bs, true);
}

auto Network::CommonSocket::connect(std::span<const std::byte> t_bs) -> OsErrorResult
{
    return open(t_bs, false);
}

auto Network::CommonSocket::listen(int t_backlog) const -> OsErrorResult
{
    return Network::listen(m_sd, t_backlog);
}

auto Network::CommonSocket::name(bool t_is_peer) const ->
    std::span<const std::byte>
{
    const auto index {static_cast<std::size_t>(t_is_peer)};
    ByteString& value {m_names.at(index)};

    if (value.empty()) {
        value = Network::get_name(m_sd, t_is_peer);
    }

    return value;
}

auto Network::CommonSocket::open(std::span<const std::byte> t_bs,
                                 bool t_is_bind) -> OsErrorResult
{
    if (const auto result {Network::open(m_sd, t_bs, t_is_bind)}) {
        return result;
    }

    return {};
}

auto Network::CommonSocket::peername() const -> std::span<const std::byte>
{
    return name(true);
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
    return name(false);
}

auto Network::CommonSocket::write(std::string_view t_sv) const -> ssize_t
{
    return Network::write(m_sd, t_sv);
}
