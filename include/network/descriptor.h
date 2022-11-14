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

#ifndef NETWORK_DESCRIPTOR_H
#define NETWORK_DESCRIPTOR_H

#include "network/bytestring.h"                 // ByteString
#include "network/fd-null.h"                    // fd_null
#include "network/fd-type.h"                    // fd_type

#include <string>      // std::string

namespace Network
{
    class Descriptor
    {
    public:
        explicit Descriptor(fd_type t_fd_data,
                            bool t_pending,
                            bool t_verbose = false) noexcept;
        Descriptor(const Descriptor&) noexcept = delete;
        Descriptor(Descriptor&&) noexcept = delete;
        ~Descriptor() noexcept;
        auto operator=(const Descriptor&) noexcept -> Descriptor& = delete;
        auto operator=(Descriptor&&) noexcept -> Descriptor& = delete;
        auto operator=(fd_type value) noexcept -> Descriptor&;
        auto close() noexcept -> Descriptor&;
        [[nodiscard]] auto handle() const noexcept -> fd_type;
        [[nodiscard]] auto verbose() const noexcept -> bool;

    private:
        fd_type m_fd {fd_null};
        bool m_pending:1 {false};
        bool m_verbose:1 {false};
    };
}

#endif
