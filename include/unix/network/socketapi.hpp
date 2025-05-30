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

#ifndef UNIX_NETWORK_SOCKETAPI_HPP
#define UNIX_NETWORK_SOCKETAPI_HPP

#ifndef WIN32

#include "network/failmode.hpp"         // FailMode
#include "network/runtime.hpp"          // Runtime
#include "network/runtimestate.hpp"     // RuntimeState
#include "network/socketapidata.hpp"    // SocketApiData

#include <string>       // std::string

namespace Network
{
    class SocketApi final : public Runtime
    {
    public:
        SocketApi(FailMode t_fail_mode, bool t_is_verbose);
        explicit SocketApi(bool t_is_verbose = false);
        SocketApi(const SocketApi&) = delete;
        SocketApi(const SocketApi&&) = delete;
        ~SocketApi() final;
        auto operator=(const SocketApi&) -> SocketApi& = delete;
        auto operator=(const SocketApi&&) -> SocketApi& = delete;
        explicit operator std::string() const final;
        [[nodiscard]] auto error_code() const noexcept -> int final;
        [[nodiscard]] auto is_running() const noexcept -> bool final;
        auto start() -> Runtime& final;
        auto stop() -> Runtime& final;

    private:
        SocketApiData m_data {};
        RuntimeState m_state {};
    };
}

#endif

#endif
