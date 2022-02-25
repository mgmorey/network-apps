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

#include "network/fd-null.h"                    // fd_null
#include "network/fd-type.h"                    // fd_type
#include "network/socket-hint-types.h"          // socket_family_type,
                                                // socket_flags_type,
                                                // socket_protocol_type,
                                                // socket_type_type
#include "network/sockethints.h"                // SocketHints

#include <ostream>     // std::ostream
#include <string>      // std::string

namespace Network
{
    class Fd
    {
    public:
        using value_type = fd_type;

        Fd(socket_family_type t_family,
           socket_type_type t_socktype,
           socket_protocol_type t_protocol = 0,
           socket_flags_type t_flags = 0,
           bool t_verbose = false);
        explicit Fd(const SocketHints& t_hints,
                    bool t_verbose = false);
        explicit Fd(value_type t_fd) noexcept;
        Fd(const Fd&) noexcept = default;
        Fd(Fd&&) noexcept = default;
        Fd() noexcept = default;
        ~Fd() noexcept = default;
        auto operator=(const Fd&) noexcept -> Fd& = default;
        auto operator=(Fd&&) noexcept -> Fd& = default;
        explicit operator bool() const noexcept;
        explicit operator value_type() const noexcept;
        explicit operator std::string() const;

    private:
        value_type m_value {fd_null};
    };

    extern auto operator<<(std::ostream& os, Fd fd) noexcept -> std::ostream&;
}

#endif
