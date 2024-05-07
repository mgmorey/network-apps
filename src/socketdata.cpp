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

#include "network/socketdata.h"                 // SocketData
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-peername.h"               // get_peername()
#include "network/get-sockname.h"               // get_sockname()
#include "network/logicerror.h"                 // LogicError
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type

#ifndef WIN32
#include "network/remove.h"                     // remove()
#include "network/to-path.h"                    // to_path()
#endif

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::endl
#include <string>       // std::to_string()

Network::SocketData::SocketData(socket_type t_socket, bool t_verbose) :
    m_socket(t_socket),
    m_verbose(t_verbose)
{
    if (m_socket == socket_null) {
        throw LogicError("Invalid socket descriptor value: " +
                         std::to_string(m_socket));
    }
}

Network::SocketData::~SocketData() noexcept
{
    try {
        reset();
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}

Network::SocketData::operator socket_type() const noexcept
{
    return m_socket;
}

auto Network::SocketData::bound() -> SocketData&
{
    return bound(true);
}

auto Network::SocketData::peername() const -> ByteString
{
    if (!m_peername) {
        m_peername = get_peername(m_socket, m_verbose);
    }

    return *m_peername;
}

auto Network::SocketData::sockname() const -> ByteString
{
    if (!m_sockname) {
        m_sockname = get_sockname(m_socket, m_verbose);
    }

    return *m_sockname;
}

auto Network::SocketData::bound(bool t_bound) -> SocketData&
{
#ifndef WIN32
    if (m_sockpath.has_value() == t_bound) {
        return *this;
    }

    if (m_sockpath) {
        remove(*m_sockpath, m_verbose);
        m_sockpath.reset();
    } else {
        m_sockpath = to_path(sockname());
    }

#else
    static_cast<void>(t_bound);
#endif
    return *this;
}

auto Network::SocketData::close() -> SocketData&
{
    if (const auto result {Network::close(m_socket, m_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }

    m_socket = socket_null;
    return *this;
}

auto Network::SocketData::reset() -> SocketData&
{
    close();
    bound(false);
    m_sockname.reset();
    m_peername.reset();
    return *this;
}
