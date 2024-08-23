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
#include "network/handle-null.h"                // handle_null
#include "network/handle-type.h"                // handle_type
#include "network/logicerror.h"                 // LogicError

#ifndef WIN32
#include "network/remove.h"                     // remove()
#include "network/to-path.h"                    // to_path()
#endif

#include <exception>    // std::exception
#include <iostream>     // std::cerr, std::endl
#include <string>       // std::to_string()

Network::SocketData::SocketData(handle_type t_handle, bool t_is_verbose) :
    m_handle(t_handle),
    m_is_verbose(t_is_verbose)
{
    if (m_handle == handle_null) {
        throw LogicError("Invalid socket descriptor value: " +
                         std::to_string(m_handle));
    }
}

Network::SocketData::~SocketData() noexcept
{
    try {
        close();
        is_owner(false);
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}

Network::SocketData::operator bool() const noexcept
{
    return m_handle != handle_null;
}

Network::SocketData::operator handle_type() const noexcept
{
    return m_handle;
}

auto Network::SocketData::handle() const noexcept -> handle_type
{
    return m_handle;
}

auto Network::SocketData::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::SocketData::peername() const -> ByteString
{
    if (!m_peername) {
        m_peername = get_peername(m_handle, m_is_verbose);
    }

    return *m_peername;
}

auto Network::SocketData::sockname() const -> ByteString
{
    if (!m_sockname) {
        m_sockname = get_sockname(m_handle, m_is_verbose);
    }

    return *m_sockname;
}

auto Network::SocketData::close() const -> const SocketData&
{
    if (const auto result {Network::close(m_handle, m_is_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }

    return *this;
}

auto Network::SocketData::is_owner() -> SocketData&
{
    return is_owner(true);
}

auto Network::SocketData::is_owner(bool t_is_owner) -> SocketData&
{
    if (m_is_owner == t_is_owner) {
        return *this;
    }

#ifndef WIN32
    if (t_is_owner) {
        if (!m_sockname) {
            m_sockname = get_sockname(m_handle, m_is_verbose);
        }
    } else if (m_sockname) {
        if (const auto path {to_path(*m_sockname)}) {
            remove(*path, m_is_verbose);
        }
    }
#endif

    m_is_owner = t_is_owner;
    return *this;
}
