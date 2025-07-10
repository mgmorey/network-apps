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

#ifndef UNIX_NETWORK_CREATE_SOCKETPAIR_HPP
#define UNIX_NETWORK_CREATE_SOCKETPAIR_HPP

#ifndef _WIN32

#include "network/sharedruntime.hpp"    // SharedRuntime
#include "network/sockethints.hpp"      // SocketHints
#include "network/socketpair.hpp"       // SocketPair

namespace Network
{
    extern auto create_socketpair(const SocketHints& hints,
                                  const SharedRuntime& sr) -> SocketPair;
    extern auto create_socketpair(const SocketHints& hints,
                                  bool verbose = false) -> SocketPair;
}

#endif

#endif
