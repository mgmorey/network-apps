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

#ifndef NETWORK_FD_H
#define NETWORK_FD_H

#ifdef _WIN32
#include <winsock2.h>       // INVALID_SOCKET, SOCKET
#endif

#include <ostream>     // std::ostream

namespace Network
{
#ifdef _WIN32
    using fd_type = SOCKET;
#else
    using fd_type = int;
#endif

#ifndef INVALID_SOCKET
#ifdef _WIN32
    constexpr fd_type INVALID_SOCKET {~0};
#else
    constexpr fd_type INVALID_SOCKET {-1};
#endif
#endif

    enum { fd_null = INVALID_SOCKET };

    class Fd
    {
        friend auto operator<<(std::ostream& os, Fd fd) -> std::ostream&;

    public:
        Fd() = default;
        Fd(const Fd& t_fd) = default;
        Fd(Fd&& t_fd) = default;
        // cppcheck-suppress noExplicitConstructor
        Fd(fd_type t_fd);  // NOLINT
        ~Fd() = default;
        auto operator=(const Fd& t_fd) -> Fd& = default;
        auto operator=(Fd&& t_fd) -> Fd& = default;
        operator fd_type() const;  // NOLINT
        explicit operator bool() const;

    private:
        fd_type m_value {fd_null};
    };

    auto operator<<(std::ostream& os, Fd fd) -> std::ostream&;
}

#endif
