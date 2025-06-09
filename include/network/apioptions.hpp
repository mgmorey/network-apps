// Copyright (C) 2025  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifndef NETWORK_APIINPUT_HPP
#define NETWORK_APIINPUT_HPP

#include "network/failmode.hpp"                 // FailMode
#include "network/optionalversion.hpp"          // OptionalVersion

namespace Network
{
    struct ApiOptions
    {
        ApiOptions(OptionalVersion t_version,
                   FailMode t_fail_mode,
                   bool t_is_verbose) noexcept;
        ApiOptions(FailMode t_fail_mode,
                   bool t_is_verbose) noexcept;
        explicit ApiOptions(bool t_is_verbose) noexcept;

        ApiOptions() noexcept = default;
        ApiOptions(const ApiOptions&) noexcept = default;
        ApiOptions(ApiOptions&&) noexcept = default;
        ~ApiOptions() noexcept = default;
        auto operator=(const ApiOptions&) noexcept -> ApiOptions& = default;
        auto operator=(ApiOptions&&) noexcept -> ApiOptions& = default;

        [[nodiscard]] auto version() const noexcept -> OptionalVersion;
        [[nodiscard]] auto fail_mode() const noexcept -> FailMode;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;

    private:
        FailMode m_fail_mode {FailMode::throw_error};
        bool m_is_verbose {false};
        OptionalVersion m_version;
    };
}

#endif
