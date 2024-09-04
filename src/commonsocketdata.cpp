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

#include "network/commonsocketdata.h"           // CommonSocketData
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-peername.h"               // get_peername()
#include "network/get-sockname.h"               // get_sockname()
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/logicerror.h"                 // LogicError
#include "network/open-handle.h"                // open()
#include "network/openhandleparams.h"           // OpenHandleParams
#include "network/openhandler.h"                // OpenHandler
#include "network/oserrorresult.h"              // OsErrorResult
#include "network/socket-family-type.h"         // socket_family_type

#ifdef WIN32
#include <winsock2.h>       // ::bind(), ::connect()
#else
#include <sys/socket.h>     // ::bind(), ::connect()
#endif

#include <array>        // std::arrray
#include <iostream>     // std::cerr, std::endl
#include <string>       // std::to_string()
#include <utility>      // std::make_pair

Network::CommonSocketData::CommonSocketData(socket_family_type t_family,
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

Network::CommonSocketData::~CommonSocketData() noexcept
{
    if (const auto result {Network::close(m_handle, m_is_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }
}

Network::CommonSocketData::operator bool() const noexcept
{
    return m_handle != handle_null;
}

Network::CommonSocketData::operator handle_type() const noexcept
{
    return m_handle;
}

auto Network::CommonSocketData::bind(const ByteString& t_addr) -> OsErrorResult
{
    return open(t_addr, true);
}

auto Network::CommonSocketData::connect(const ByteString& t_addr) -> OsErrorResult
{
    return open(t_addr, false);
}

auto Network::CommonSocketData::family() const noexcept -> socket_family_type
{
    return m_family;
}

auto Network::CommonSocketData::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::CommonSocketData::is_owner(bool t_is_owner) -> SocketData&
{
    static_cast<void>(t_is_owner);
    return *this;
}

auto Network::CommonSocketData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::CommonSocketData::open(const ByteString& t_addr,
                                     bool t_is_bind) -> OsErrorResult
{
    static const std::array<OpenHandler, 2> handler
    {
        std::make_pair(::connect, "connect"),
        std::make_pair(::bind, "bind"),
    };

    const OpenHandleParams args {m_handle, t_addr, m_is_verbose};
    const auto index {static_cast<std::size_t>(t_is_bind)};

    if (auto result {Network::open(handler.at(index), args)}) {
        return result;
    }

    if (t_is_bind) {
        is_owner();
    }

    return {};
}

auto Network::CommonSocketData::peername() const -> ByteString
{
    if (!m_peername) {
        m_peername = Network::get_peername(m_handle, m_is_verbose);
    }

    return *m_peername;
}

auto Network::CommonSocketData::sockname() const -> ByteString
{
    if (!m_sockname) {
        m_sockname = Network::get_sockname(m_handle, m_is_verbose);
    }

    return *m_sockname;
}
