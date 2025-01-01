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

#ifndef UNIX_NETWORK_UNIXCONTEXT_H
#define UNIX_NETWORK_UNIXCONTEXT_H

#ifndef WIN32

#include "network/context.h"                    // Context
#include "network/contextdata.h"                // ContextData
#include "network/failuremode.h"                // FailureMode
#include "network/optionalversion.h"            // OptionalVersion

#include <string>       // std::string

namespace Network
{
    class NativeContext final
        : public Context
    {
    public:
        NativeContext(const OptionalVersion& t_version,
                      FailureMode t_failure,
                      bool t_is_verbose = false);
        explicit NativeContext(bool t_is_verbose = false);
        NativeContext(const NativeContext&) = delete;
        NativeContext(const NativeContext&&) = delete;
        ~NativeContext() final;
        auto operator=(const NativeContext&) -> NativeContext& = delete;
        auto operator=(const NativeContext&&) -> NativeContext& = delete;
        explicit operator std::string() const final;
        [[nodiscard]] auto error_code() const noexcept -> int final;
        [[nodiscard]] auto is_running() const noexcept -> bool final;
        auto start() -> Context& final;
        auto stop() -> Context& final;
        [[nodiscard]] auto version() const -> OptionalVersion final;

    private:
        ContextData m_data;
        OptionalVersion m_version;
        int m_error_code {0};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };
}

#endif

#endif
