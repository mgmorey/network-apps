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

#ifndef NETWORK_RUNTIMEDATA_HPP
#define NETWORK_RUNTIMEDATA_HPP

#include "network/failmode.hpp"                 // FailMode
#include "network/optionalversion.hpp"          // OptionalVersion

namespace Network
{
    struct RuntimeData
    {
        RuntimeData(OptionalVersion t_version,
                    FailMode t_fail_mode,
                    bool t_is_verbose) noexcept;
        RuntimeData(FailMode t_fail_mode,
                    bool t_is_verbose) noexcept;
        explicit RuntimeData(bool t_is_verbose) noexcept;

        RuntimeData() noexcept = default;
        RuntimeData(const RuntimeData&) noexcept = default;
        RuntimeData(RuntimeData&&) noexcept = default;
        ~RuntimeData() noexcept = default;
        auto operator=(const RuntimeData&) noexcept -> RuntimeData& = default;
        auto operator=(RuntimeData&&) noexcept -> RuntimeData& = default;

        [[nodiscard]] auto fail_mode() const noexcept -> FailMode;
        [[nodiscard]] auto is_verbose() const noexcept -> bool;
        [[nodiscard]] auto version() const noexcept -> OptionalVersion;

    private:
        FailMode m_fail_mode {FailMode::throw_error};       // NOLINT
        bool m_is_verbose {false};                          // NOLINT
        OptionalVersion m_version;                          // NOLINT
    };
}

#endif
