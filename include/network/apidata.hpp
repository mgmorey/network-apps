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

#ifndef NETWORK_SOCKETAPIDATA_HPP
#define NETWORK_SOCKETAPIDATA_HPP

#include "network/nativeapidata.hpp"    // NativeApiData
#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

namespace Network
{
    struct ApiData : public NativeApiData
    {
        ApiData() noexcept : NativeApiData {} {}

        ApiData(const ApiData&) noexcept = default;
        ApiData(ApiData&&) noexcept = default;
        ~ApiData() noexcept = default;
        auto operator=(const ApiData&) noexcept -> ApiData& = default;
        auto operator=(ApiData&&) noexcept -> ApiData& = default;

        [[nodiscard]] auto version() const noexcept -> Version;
        [[nodiscard]] auto high_version() const noexcept -> Version;
        [[nodiscard]] auto description() const noexcept -> std::string_view;
        [[nodiscard]] auto system_status() const noexcept -> std::string_view;
    };
}

#endif
