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

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Runtime
    {
    public:
        Runtime() noexcept = default;
        Runtime(const Runtime&) = delete;
        Runtime(const Runtime&&) = delete;
        virtual ~Runtime() = default;
        auto operator=(const Runtime&) -> Runtime& = delete;
        auto operator=(const Runtime&&) -> Runtime& = delete;
        explicit virtual operator std::string() const = 0;
        [[nodiscard]] virtual auto error_code() const noexcept -> int = 0;
        [[nodiscard]] virtual auto is_running() const noexcept -> bool = 0;
        virtual auto start() -> Runtime& = 0;
        virtual auto stop() -> Runtime& = 0;
    };

    extern auto operator<<(std::ostream& os,
                           const Runtime& runtime) -> std::ostream&;
}

#endif
