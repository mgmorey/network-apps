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

#ifndef NETWORK_SOCKETAPI_HPP
#define NETWORK_SOCKETAPI_HPP

#include "network/apidata.hpp"          // ApiData
#include "network/apiinput.hpp"         // ApiInput
#include "network/apistate.hpp"         // ApiState
#include "network/runtime.hpp"          // Runtime
#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

namespace Network
{
    class SocketApi final : public Runtime
    {
    public:
        explicit SocketApi(const ApiInput& t_api_input);

        SocketApi(const SocketApi&) = delete;
        SocketApi(SocketApi&&) = delete;
        ~SocketApi() final;
        auto operator=(const SocketApi&) -> SocketApi& = delete;
        auto operator=(SocketApi&&) -> SocketApi& = delete;

        [[nodiscard]] auto version() const noexcept -> Version final;
        [[nodiscard]] auto high_version() const noexcept -> Version final;
        [[nodiscard]] auto description() const noexcept ->
            std::string_view final;
        [[nodiscard]] auto system_status() const noexcept ->
            std::string_view final;
        [[nodiscard]] auto error_code() const noexcept -> int final;
        [[nodiscard]] auto is_started() const noexcept -> bool final;

        auto start() -> void final;
        auto stop() -> void final;

    private:
        ApiData m_api_data;
        ApiInput m_api_input;
        ApiState m_api_state;
    };
}

#endif
