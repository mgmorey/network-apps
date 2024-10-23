// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_UNIXSOCKET_H
#define NETWORK_UNIXSOCKET_H

#ifndef WIN32

#include "network/commonsocket.h"               // CommonSocket
#include "network/family-type.h"                // family_type
#include "network/handle-type.h"                // handle_type
#include "network/pathname.h"                   // Pathname
#include "network/pathnameview.h"               // PathnameView
#include "network/socketstate.h"                // SocketState

namespace Network
{
    class UnixSocket final
        : public CommonSocket
    {
    public:
        UnixSocket() noexcept = default;
        UnixSocket(family_type t_family,
                   handle_type t_handle,
                   bool t_is_verbose);
        UnixSocket(const UnixSocket&) noexcept = delete;
        UnixSocket(const UnixSocket&&) noexcept = delete;
        ~UnixSocket() noexcept final;
        auto operator=(const UnixSocket&) noexcept -> UnixSocket& = delete;
        auto operator=(UnixSocket&&) noexcept -> UnixSocket& = delete;

        [[nodiscard]] auto close() -> OsErrorResult final;
        [[nodiscard]] auto open(const ByteString& t_addr,
                                bool t_is_bind) -> OsErrorResult final;

    protected:
        [[nodiscard]] auto remove(const PathnameView& t_path) const -> bool;
        auto state(SocketState t_state) -> UnixSocket&;

    private:
        Pathname m_path;
        SocketState m_state {SocketState::allocated};
    };
}

#endif

#endif
