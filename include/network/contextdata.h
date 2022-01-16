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

#ifndef NETWORK_CONTEXT_DATA_H
#define NETWORK_CONTEXT_DATA_H

#include "network/version-type.h"       // version_type

#ifdef _WIN32
#include <winsock2.h>       // WSADATA
#endif

namespace Network
{
#ifdef _WIN32
    using ContextData = WSADATA;
#else
    struct ContextData
    {
        std::string m_status {};
        std::string m_system {};
        version_type m_version {0};
    };
#endif
}

#endif
