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

#include <ostream>      // std::ostream
#include <string>       // std::string

namespace Network
{
    class Context
    {
        friend auto operator<<(std::ostream& os,
                               const Context& context) -> std::ostream&;

    public:
        Context(const OptionalVersion& t_version,
                FailureMode t_failure,
                bool t_is_verbose = false);
        Context(const Context&) = delete;
        Context(const Context&&) = delete;
        ~Context();
        auto operator=(const Context&) -> Context& = delete;
        auto operator=(const Context&&) -> Context& = delete;
        [[nodiscard]] auto error_code() const noexcept -> int;
        [[nodiscard]] auto is_running() const noexcept -> bool;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        auto start() -> Context&;
        auto stop() -> Context&;

        [[nodiscard]] auto system_status() const -> std::string
        {
            return m_data.m_system_status;
        }
        [[nodiscard]] auto version() const -> OptionalVersion
        {
            return m_data.m_version;
        }

    protected:
        auto shut_down() const -> void;
        auto start_up() -> void;

    private:
        ContextData m_data;
        OptionalVersion m_version;
        int m_error_code {0};
        FailureMode m_failure {FailureMode::throw_error};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };

    extern auto operator<<(std::ostream& os,
                           const Context& context) -> std::ostream&;
}

#endif
