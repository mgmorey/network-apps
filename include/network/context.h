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

#ifndef NETWORK_CONTEXT_H
#define NETWORK_CONTEXT_H

#include "network/optionalversion.h"            // OptionalVersion

#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;

    public:
        Context() = default;
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        virtual ~Context() = default;
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        explicit virtual operator std::string() const = 0;
        [[nodiscard]] virtual auto error_code() const noexcept -> int = 0;
        [[nodiscard]] virtual auto is_running() const noexcept -> bool = 0;
        [[nodiscard]] virtual auto is_verbose() const noexcept -> bool = 0;
        virtual auto start() -> Context* = 0;
        virtual auto stop() -> Context* = 0;
        [[nodiscard]] virtual auto version() const -> OptionalVersion = 0;
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
