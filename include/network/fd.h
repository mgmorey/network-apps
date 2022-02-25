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

#include "network/fd-null.h"            // fd_null
#include "network/fd-type.h"            // fd_type
#include "network/sockethints.h"        // SocketHints

#include <ostream>     // std::ostream
#include <string>      // std::string

namespace Network
{
    class Fd
    {
    public:
        explicit Fd(const SocketHints& t_hints,
           bool t_verbose = false) noexcept;
        explicit Fd(fd_type t_fd) noexcept;
        Fd(const Fd&) noexcept = default;
        Fd(Fd&&) noexcept = default;
        Fd() noexcept = default;
        ~Fd() noexcept = default;
        auto operator=(const Fd&) noexcept -> Fd& = default;
        auto operator=(Fd&&) noexcept -> Fd& = default;
        explicit operator fd_type() const noexcept;
        explicit operator bool() const noexcept;
        explicit operator std::string() const noexcept;

    private:
        fd_type m_value {fd_null};
    };

    extern auto operator<<(std::ostream& os, Fd fd) noexcept -> std::ostream&;
}

#endif
