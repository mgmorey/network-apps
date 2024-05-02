// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/descriptor.h"                 // Descriptor
#include "network/bytestring.h"                 // ByteString
#include "network/close.h"                      // close()
#include "network/get-peername.h"               // get_peername()
#include "network/get-sockname.h"               // get_sockname()
#include "network/socket-null.h"                // socket_null
#include "network/socket-type.h"                // socket_type
#ifndef WIN32
#include "network/remove.h"                     // remove()
#include "network/to-path-bytestring.h"         // to_path()
#endif

#ifndef WIN32
#include <exception>    // std::exception
#endif
#include <iostream>     // std::cerr, std::endl

Network::Descriptor::Descriptor(socket_type t_handle,
                                bool t_verbose) noexcept :
    m_handle(t_handle),
    m_verbose(t_verbose)
{
}

Network::Descriptor::~Descriptor() noexcept
{
    try {
        static_cast<void>(close());
    }
    catch (const std::exception& error) {
        std::cerr << error.what()
                  << std::endl;
    }
}

auto Network::Descriptor::operator=(socket_type t_handle) noexcept ->
    Descriptor&
{
    m_handle = t_handle;
    return *this;
}

Network::Descriptor::operator socket_type() const noexcept
{
    return m_handle;
}

Network::Descriptor::operator bool() const noexcept
{
    return m_handle != socket_null;
}

auto Network::Descriptor::bound(bool t_bound) -> Descriptor&
{
#ifndef WIN32
    if (m_pathname.has_value() != t_bound) {
        if (m_pathname) {
            remove(*m_pathname, m_verbose);
            m_pathname.reset();
        } else {
            m_pathname = to_path(sockname());
        }
    }
#else
    static_cast<void>(t_bound);
#endif
    return *this;
}

auto Network::Descriptor::close() -> Descriptor&
{
    if (m_handle == socket_null) {
        return *this;
    }

    if (const auto result {Network::close(m_handle, m_verbose)}) {
        std::cerr << result.string()
                  << std::endl;
    }
    else {
        m_handle = socket_null;
    }

    bound(false);
    return *this;
}

auto Network::Descriptor::handle() const noexcept -> socket_type
{
    return m_handle;
}

auto Network::Descriptor::peername() const -> ByteString
{
    if (!m_peername) {
        m_peername = get_peername(m_handle, m_verbose);
    }

    return *m_peername;
}

auto Network::Descriptor::sockname() const -> ByteString
{
    if (!m_sockname) {
        m_sockname = get_sockname(m_handle, m_verbose);
    }

    return *m_sockname;
}

auto Network::Descriptor::verbose() const noexcept -> bool
{
    return m_verbose;
}
