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

// This UNIX domain socket example is adapted from the example in
// https://www.man7.org/linux/man-pages/man7/unix.7.html.

// NOLINTNEXTLINE(llvm-header-guard)
#ifndef UNIX_CONNECTION_HPP
#define UNIX_CONNECTION_HPP

#include "network/socket.hpp"          // Socket
#include "network/unixsockethints.hpp" // UnixSocketHints

#include <cstddef>      // std::size_t
#include <string>       // std::string
#include <string_view>  // std::string_view
#include <sys/socket.h> // SOCK_SEQPACKET, SOCK_STREAM

namespace NetworkExamples::Unix
{
    inline constexpr std::size_t message_size_max {64};
#ifdef __APPLE__
    inline constexpr Network::UnixSocketHints socket_hints {SOCK_STREAM};
    inline constexpr auto uses_stream_socket {true};
#else
    inline constexpr Network::UnixSocketHints socket_hints {SOCK_SEQPACKET};
    inline constexpr auto uses_stream_socket {false};
#endif
    inline constexpr const char* socket_name {"/tmp/9Lq7BNBnBycd6nxy.socket"};

    extern auto read_message(const Network::Socket& socket) -> std::string;
    extern auto write_message(const Network::Socket& socket,
                              std::string_view message) -> void;
} // namespace NetworkExamples::Unix

#endif
