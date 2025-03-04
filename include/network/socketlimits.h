// Copyright (C) 2023  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKETLIMITS_H
#define NETWORK_SOCKETLIMITS_H

#include "network/family-null.h"        // family_null
#include "network/family-type.h"        // family_type

#include <cstddef>      // std::size_t
#include <utility>      // std::pair

namespace Network
{
    class SocketLimits
    {
    public:
        explicit SocketLimits(family_type t_family);

        SocketLimits() noexcept = default;
        SocketLimits(const SocketLimits&) noexcept = default;
        SocketLimits(SocketLimits&&) noexcept = default;
        ~SocketLimits() noexcept = default;
        auto operator=(const SocketLimits&) noexcept -> SocketLimits& = default;
        auto operator=(SocketLimits&&) noexcept -> SocketLimits& = default;

        [[nodiscard]] auto limits() const noexcept ->
            std::pair<std::size_t, std::size_t>;
        [[nodiscard]] auto max() const noexcept -> std::size_t;
        [[nodiscard]] auto min() const noexcept -> std::size_t;

    private:
        family_type m_family {family_null};
    };
}

#endif
