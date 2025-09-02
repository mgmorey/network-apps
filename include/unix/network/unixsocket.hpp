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

#ifndef UNIX_NETWORK_UNIXSOCKET_HPP
#define UNIX_NETWORK_UNIXSOCKET_HPP

#ifndef _WIN32

#include "network/bytespan.hpp"         // ByteSpan
#include "network/inetsocket.hpp"       // InetSocket
#include "network/opensymbol.hpp"       // OpenSymbol
#include "network/oserror.hpp"          // OsError
#include "network/pathname.hpp"         // Pathname
#include "network/pathnameview.hpp"     // PathnameView
#include "network/socketdata.hpp"       // SocketData
#include "network/socketstate.hpp"      // SocketState

namespace Network
{
    class UnixSocket final : public InetSocket
    {
    public:
        explicit UnixSocket(const SocketData& t_sd);
        UnixSocket(const UnixSocket&) noexcept = delete;
        UnixSocket(UnixSocket &&) noexcept = delete;
        ~UnixSocket() noexcept final;
        auto operator=(const UnixSocket&) noexcept -> UnixSocket& = delete;
        auto operator=(UnixSocket&&) noexcept -> UnixSocket& = delete;

        [[nodiscard]] auto open(ByteSpan t_bs, OpenSymbol t_symbol) ->
            OsError final;

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
