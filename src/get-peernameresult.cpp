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

#include "network/get-peernameresult.h" // Fd, GetNameHandler,
                                        // GetNameParams,
                                        // PeerNameResult, fd_type,
                                        // get_peernameresult()
#include "network/get-nameresult.h"     // get_nameresult()

#ifdef WIN32
#include <winsock2.h>   // ::getpeername()
#else
#include <sys/socket.h> // ::getpeername()
#endif

auto Network::get_peernameresult(fd_type handle, bool verbose) ->
    Network::PeerNameResult
{
    const GetNameHandler handler {::getpeername, "::getpeername"};
    const GetNameParams args {handle, verbose};
    return get_nameresult(handler, args);
}
