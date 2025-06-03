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

#ifndef WINDOWS_NETWORK_SOCKETAPIDATA_HPP
#define WINDOWS_NETWORK_SOCKETAPIDATA_HPP

#ifdef WIN32

#include "network/version.hpp"          // Version
#include "network/windowsversion.hpp"   // WindowsVersion

#include <winsock2.h>       // WSAData

#include <cstring>      // std::memset()
#include <string_view>  // std::string_view

namespace Network
{
    struct SocketApiData : public WSAData
    {
        constexpr SocketApiData() :
             WSAData {.wVersion = 0,
                      .wHighVersion = 0,
                      .iMaxSockets = 0,
                      .iMaxUdpDg = 0,
                      .lpVendorInfo = nullptr,
                      .szDescription = "",
                      .szSystemStatus = ""}
        {
        }

        constexpr SocketApiData(const SocketApiData&) noexcept = default;
        constexpr SocketApiData(SocketApiData&&) noexcept = default;
        constexpr ~SocketApiData() noexcept = default;
        constexpr auto operator=(const SocketApiData&) noexcept ->
                SocketApiData& = default;
        constexpr auto operator=(SocketApiData&&) noexcept ->
                SocketApiData& = default;

        [[nodiscard]] auto version() const noexcept -> Version
        {
            return WindowsVersion {wVersion};
        }

        [[nodiscard]] auto description() const noexcept -> std::string_view
        {
            return static_cast<const char*>(szDescription);
        }

        [[nodiscard]] auto system_status() const noexcept -> std::string_view
        {
            return static_cast<const char*>(szSystemStatus);
        }
    };
}

#endif

#endif
