// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_SOCKETDATA_HPP
#define NETWORK_SOCKETDATA_HPP

#include "network/family-null.hpp"      // family_null
#include "network/family-type.hpp"      // family_type
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type

namespace Network
{
    class SocketData
    {
    public:
        explicit SocketData(handle_type t_handle,
                            family_type t_family = family_null,
                            bool t_is_verbose = false);
        SocketData(const SocketData& t_sd,
                   handle_type t_handle);

        SocketData(const SocketData&) noexcept = default;
        SocketData(SocketData&&) noexcept = default;
        ~SocketData() noexcept = default;
        auto operator=(const SocketData&) noexcept -> SocketData& = default;
        auto operator=(SocketData&&) noexcept -> SocketData& = default;

        [[nodiscard]] auto family() const noexcept -> family_type;
        [[nodiscard]] auto handle() const noexcept -> handle_type;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;

    protected:
        SocketData() = default;

        auto family(family_type t_family) -> SocketData&;
        auto handle(handle_type t_handle) -> SocketData&;
        auto is_verbose(bool t_is_verbose) -> SocketData&;

    private:
        handle_type m_handle {handle_null};
        family_type m_family {family_null};
        bool m_is_verbose {false};
    };
}

#endif
