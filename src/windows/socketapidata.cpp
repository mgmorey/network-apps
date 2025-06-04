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

#ifdef WIN32

#include "network/socketapidata.hpp"    // SocketApiData
#include "network/apidata.hpp"          // ApiData
#include "network/windowsversion.hpp"   // WindowsVersion

Network::SocketApiData::SocketApiData() : ApiData {}
{
}

auto Network::SocketApiData::version() const noexcept -> Version
{
    return WindowsVersion {wVersion};
}

auto Network::SocketApiData::high_version() const noexcept -> Version
{
    return WindowsVersion {wHighVersion};
}

auto Network::SocketApiData::description() const noexcept -> std::string_view
{
    return static_cast<const char*>(szDescription);
}

auto Network::SocketApiData::system_status() const noexcept -> std::string_view
{
    return static_cast<const char*>(szSystemStatus);
}

#endif
