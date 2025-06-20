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

#include "network/get-namehandler.hpp"  // get_namehandler()
#include "network/namehandler.hpp"      // NameHandler

#ifdef WIN32
#include <winsock2.h>       // ::getpeername(), ::getsockname()
#else
#include <sys/socket.h>     // ::getpeername(), ::getsockname()
#endif

#include <array>        // std::arrray
#include <cstddef>      // std::size_t
#include <utility>      // std::make_pair

auto Network::get_namehandler(bool is_sockname) -> NameHandler
{
    static const std::array<NameHandler, 2> handlers {
        std::make_pair(::getpeername, "::getpeername"),
        std::make_pair(::getsockname, "::getsockname"),
    };

    return handlers.at(static_cast<std::size_t>(is_sockname));
}
