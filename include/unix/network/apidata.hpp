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

#ifndef UNIX_NETWORK_APIDATA_HPP
#define UNIX_NETWORK_APIDATA_HPP

#ifndef _WIN32

#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

namespace Network
{
    struct ApiData
    {
        Version m_version;
        Version m_high_version;
        std::string_view m_description;
        std::string_view m_system_status;
    };
}

#endif

#endif
