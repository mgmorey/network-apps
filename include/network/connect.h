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

#ifndef NETWORK_CONNECT_H
#define NETWORK_CONNECT_H

#include "network/open.h"           // Bytes, Endpoint, Fd,
                                    // FdResultVector, Hints, Result

namespace Network
{
    extern auto connect(Fd fd,
                        const Bytes& addr,
                        bool verbose = false) -> Result;
    extern auto connect(const Endpoint& endpoint,
                        const Hints& hints,
                        bool verbose = false) -> FdResultVector;
}

#endif
