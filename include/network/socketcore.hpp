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

#ifndef NETWORK_SOCKETCORE_HPP
#define NETWORK_SOCKETCORE_HPP

#include "network/family-null.hpp"      // family_null
#include "network/family-type.hpp"      // family_type
#include "network/handle-null.hpp"      // handle_null
#include "network/handle-type.hpp"      // handle_type
#include "network/sharedruntime.hpp"    // SharedRuntime

namespace Network
{
    class SocketCore
    {
    public:
        SocketCore(handle_type t_handle,
                   family_type t_family,
                   const SharedRuntime& t_sr);
        SocketCore(const SocketCore& t_core,
                   handle_type t_handle);

        SocketCore() = delete;
        SocketCore(const SocketCore&) noexcept = default;
        SocketCore(SocketCore&&) noexcept = default;
        ~SocketCore() noexcept = default;
        auto operator=(const SocketCore&) noexcept -> SocketCore& = default;
        auto operator=(SocketCore&&) noexcept -> SocketCore& = default;

        [[nodiscard]] auto family() const noexcept -> family_type;
        [[nodiscard]] auto handle() const noexcept -> handle_type;
        [[nodiscard]] auto runtime() const noexcept -> SharedRuntime;

    private:
        SharedRuntime m_sr;
        handle_type m_handle {handle_null};
        family_type m_family {family_null};
    };
}

#endif
