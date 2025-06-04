// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/socketapidata.hpp"    // SocketApiData

#ifndef WIN32

Network::SocketApiData::SocketApiData() : ApiData {}
{
}

auto Network::SocketApiData::version() const noexcept -> Version
{
    return m_version;
}

auto Network::SocketApiData::high_version() const noexcept -> Version
{
    return m_high_version;
}

auto Network::SocketApiData::description() const noexcept -> std::string_view
{
    return m_description;
}

auto Network::SocketApiData::system_status() const noexcept -> std::string_view
{
    return m_system_status;
}

#endif
