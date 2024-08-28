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

#include "network/contextdata.h"                // ContextData
#include "network/failuremode.h"                // FailureMode
#include "network/optionalversion.h"            // OptionalVersion

#include <memory>       // std::shared_ptr
#include <ostream>      // std::ostream

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;

    public:
        using Instance = std::shared_ptr<const Context>;

        static const auto m_failsafe {FailureMode::return_zero};
        static auto instance(const OptionalVersion& t_version = {},
                             bool t_is_verbose = false) -> Instance;

        explicit Context(const OptionalVersion& t_version = {},
                         bool t_is_verbose = false);
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto error_code() const noexcept -> int;
        [[nodiscard]] auto is_running() const noexcept -> bool;
        [[nodiscard]] auto is_started() const noexcept -> bool;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        auto start(const OptionalVersion& t_version) -> Context&;
        auto stop(FailureMode mode) -> Context&;

    private:
        ContextData m_data;
        int m_error_code {0};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
