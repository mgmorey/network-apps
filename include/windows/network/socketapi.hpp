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

#ifndef WINDOWS_NETWORK_SOCKETAPI_HPP
#define WINDOWS_NETWORK_SOCKETAPI_HPP

#ifdef WIN32

#include "network/failmode.hpp"         // FailMode
#include "network/optionalversion.hpp"  // OptionalVersion
#include "network/runtime.hpp"          // Runtime
#include "network/socketapidata.hpp"    // SocketApiData
#include "network/version.hpp"          // Version

#include <string>       // std::string
#include <string_view>  // std::string_view

namespace Network
{
    class SocketApi final : public Runtime
    {
    public:
        SocketApi(Version t_version,
                  FailMode t_fail_mode,
                  bool t_is_verbose);
        SocketApi(FailMode t_fail_mode,
                  bool t_is_verbose);
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
        std::string_view m_description;
        std::string_view m_system_status;
        OptionalVersion m_version;
        int m_error_code {0};
        FailMode m_fail_mode {FailMode::throw_error};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };
}

#endif

#endif
