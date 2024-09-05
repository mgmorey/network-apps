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
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-name.h"                   // get_name()
#include "network/getnameparams.h"              // GetNameParams
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/listen.h"                     // listen()
#include "network/logicerror.h"                 // LogicError
#include "network/open-handle.h"                // open()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket-family-type.h"         // socket_family_type

#include <iostream>     // std::cerr, std::endl
#include <string>       // std::to_string()

Network::CommonSocket::CommonSocket(socket_family_type t_family,
                                    handle_type t_handle,
                                    bool t_is_verbose)
    : m_family(t_family),
      m_handle(t_handle),
      m_is_verbose(t_is_verbose)
{
    if (m_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value: " +
                         std::to_string(m_handle));
    }
}

Network::CommonSocket::~CommonSocket() noexcept
{
    if (const auto result {Network::close(m_handle, m_is_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }
}

Network::CommonSocket::operator bool() const noexcept
{
    return m_handle != handle_null;
}

Network::CommonSocket::operator handle_type() const noexcept
{
    return m_handle;
}

auto Network::CommonSocket::family() const noexcept -> socket_family_type
{
    return m_family;
}

auto Network::CommonSocket::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::CommonSocket::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::CommonSocket::listen(int t_backlog_size) const -> OsErrorResult
{
    return Network::listen(m_handle, t_backlog_size, m_is_verbose);
}

auto Network::CommonSocket::name(bool t_is_peer) const -> ByteString
{
    const auto index {static_cast<std::size_t>(t_is_peer)};
    ByteString& value {m_names.at(index)};

    if (value.empty()) {
        const GetNameParams args {m_handle, m_is_verbose};
        value = Network::get_name(args, t_is_peer);
    }

    return value;
}

auto Network::CommonSocket::open(const ByteString& t_addr,
                                     bool t_is_bind) -> OsErrorResult
{
    const OpenHandleParams args {m_handle, t_addr, m_is_verbose};

    if (auto result {Network::open(args, t_is_bind)}) {
        return result;
    }

    if (t_is_bind) {
        state(SocketState::bound);
    }
    else {
        state(SocketState::connected);
    }

    return {};
}

auto Network::CommonSocket::state(SocketState t_state) -> GenericSocket&
{
    m_state = t_state;
    return *this;
}
