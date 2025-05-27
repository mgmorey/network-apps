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

#ifndef WINDOWS_NETWORK_NATIVECONTEXT_HPP
#define WINDOWS_NETWORK_NATIVECONTEXT_HPP

#ifdef WIN32

#include "network/context.hpp"          // Context
#include "network/contextdata.hpp"      // ContextData
#include "network/failuremode.hpp"      // FailureMode
#include "network/optionalversion.hpp"  // OptionalVersion
#include "network/version.hpp"          // Version

#include <string>       // std::string
#include <string_view>  // std::string_view

namespace Network
{
    class NativeContext final : public Context
    {
    public:
        NativeContext(Version t_version,
                      FailureMode t_failure,
                      bool t_is_verbose);
        NativeContext(FailureMode t_failure,
                      bool t_is_verbose);
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
        ContextData m_data {};
        std::string_view m_description;
        std::string_view m_system_status;
        OptionalVersion m_version;
        int m_error_code {0};
        FailureMode m_failure {FailureMode::throw_error};
        bool m_is_started {false};
        bool m_is_verbose {false};
    };
}

#endif

#endif
