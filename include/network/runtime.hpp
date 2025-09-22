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

#ifndef NETWORK_RUNTIME_HPP
#define NETWORK_RUNTIME_HPP

#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

namespace Network
{
    class Runtime
    {
    public:
        Runtime() noexcept = default;
        Runtime(const Runtime&) = delete;
        Runtime(Runtime &&) = delete;
        virtual ~Runtime() = default;
        auto operator=(const Runtime&) -> Runtime& = delete;
        auto operator=(Runtime &&) -> Runtime& = delete;

        [[nodiscard]] virtual auto version() const noexcept -> Version = 0;
        [[nodiscard]] virtual auto high_version() const noexcept -> Version = 0;
        [[nodiscard]] virtual auto description() const noexcept ->
            std::string_view = 0;
        [[nodiscard]] virtual auto system_status() const noexcept ->
            std::string_view = 0;
        [[nodiscard]] virtual auto is_running() const noexcept -> bool = 0;
        [[nodiscard]] virtual auto is_verbose() const noexcept -> bool = 0;

        virtual auto start() -> bool = 0;
        virtual auto stop() -> int = 0;
    };
}

#endif
