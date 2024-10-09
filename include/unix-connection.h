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

// This UNIX domain sequenced-packet socket example is adapted from the
// example in https://www.man7.org/linux/man-pages/man7/unix.7.html.

#ifndef UNIX_CONNECTION_H
#define UNIX_CONNECTION_H

#include "network/unixsockethints.h"    // UnixSocketHints

constexpr auto BUFFER_SIZE {12};
constexpr auto SOCKET_NAME {"/tmp/9Lq7BNBnBycd6nxy.socket"};
constexpr Network::UnixSocketHints SOCKET_HINTS {SOCK_SEQPACKET};

#endif
