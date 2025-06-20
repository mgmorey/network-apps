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

#include "network/get-openhandler.hpp"  // get_openhandler()
#include "network/openhandler.hpp"      // OpenHandler

#ifdef WIN32
#include <winsock2.h>       // ::bind(), ::connect()
#else
#include <sys/socket.h>     // ::bind(), ::connect()
#endif

#include <array>        // std::arrray
#include <cstddef>      // std::size_t
#include <utility>      // std::make_pair

auto Network::get_openhandler(bool is_bind) -> OpenHandler
{
    static const std::array<OpenHandler, 2> handlers {
        std::make_pair(::connect, "::connect"),
        std::make_pair(::bind, "::bind"),
    };

    return handlers.at(static_cast<std::size_t>(is_bind));
}
