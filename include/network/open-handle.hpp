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

#ifndef NETWORK_OPEN_HANDLE_HPP
#define NETWORK_OPEN_HANDLE_HPP

#include "network/openhandler.hpp"      // OpenHandler
#include "network/oserrorresult.hpp"    // OsErrorResult
#include "network/socketdata.hpp"       // SocketData

#include <cstddef>      // std::byte
#include <span>         // std::span

namespace Network
{
    extern auto open(const SocketData& sd,
                     std::span<const std::byte> bs,
                     const OpenHandler& oh) -> OsErrorResult;
}

#endif
