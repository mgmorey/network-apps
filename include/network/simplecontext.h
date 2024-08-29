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

#ifndef NETWORK_SIMPLECONTEXT_H
#define NETWORK_SIMPLECONTEXT_H

#include "network/context.h"                    // Context
#include "network/contextdata.h"                // ContextData
#include "network/failuremode.h"                // FailureMode
#include "network/optionalversion.h"            // OptionalVersion

#include <string>       // std::string

namespace Network
{
    class SimpleContext
        : public Context
    {
    public:
        SimpleContext(const OptionalVersion& t_version,
                       FailureMode t_failure,
                       bool t_is_verbose = false);
        explicit SimpleContext(bool t_is_verbose = false);
        SimpleContext(const SimpleContext&) = delete;
        SimpleContext(const SimpleContext&&) = delete;
        ~SimpleContext() override;
        auto operator=(const SimpleContext&) -> SimpleContext& = delete;
        auto operator=(const SimpleContext&&) -> SimpleContext& = delete;
        [[nodiscard]] auto error_code() const noexcept -> int override;
        [[nodiscard]] auto is_running() const noexcept -> bool override;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        auto start() -> Context* override;
        auto stop() -> Context* override;
        [[nodiscard]] auto to_string() const -> std::string override;

        [[nodiscard]] auto version() const -> OptionalVersion override
        {
            return m_data.m_version;
        }

    protected:
        auto shut_down() const -> void;

        [[nodiscard]] auto system_status() const -> std::string
        {
            return m_data.m_system_status;
        }

    private:
        ContextData m_data;
        OptionalVersion m_version;
        int m_error_code {0};
        FailureMode m_failure {FailureMode::throw_error};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };
}

#endif
