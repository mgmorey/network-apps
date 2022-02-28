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

#include "network/fddata.h"             // Fd, FdData, fd_type,
                                        // operator<<(), std::ostream,
                                        // std::to_string()
#include "network/close.h"              // close()
#include "network/get-peername.h"       // get_peername()
#include "network/get-sockname.h"       // get_sockname()
#include "network/string-null.h"        // string_null

Network::FdData::FdData(fd_type t_fd_data,
                        bool t_pending,
                        bool t_verbose) noexcept :
    m_handle(t_fd_data),
    m_pending(t_pending),
    m_verbose(t_verbose)
{
}

Network::FdData::~FdData()
{
    static_cast<void>(this->close());
}

auto Network::FdData::operator=(fd_type value) noexcept -> FdData&
{
    m_handle = value;
    return *this;
}

Network::FdData::operator bool() const noexcept
{
    return m_handle != fd_null;
}

Network::FdData::operator fd_type() const noexcept
{
    return m_handle;
}

Network::FdData::operator std::string() const
{
    if (m_handle == fd_null) {
        return string_null;
    }

    return std::to_string(m_handle);
}

auto Network::FdData::close() -> FdData&
{
    m_handle = Network::close(m_handle, m_verbose);
    return *this;
}

auto Network::FdData::handle() const noexcept -> fd_type
{
    return m_handle;
}

auto Network::FdData::peername() const -> ByteString
{
    return get_peername(m_handle, m_verbose);
}

auto Network::FdData::sockname() const -> ByteString
{
    return get_sockname(m_handle, m_verbose);
}

auto Network::FdData::verbose() const noexcept -> bool
{
    return m_verbose;
}
