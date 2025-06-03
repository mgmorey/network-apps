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

#ifndef UNIX_NETWORK_SOCKETAPIDATA_HPP
#define UNIX_NETWORK_SOCKETAPIDATA_HPP

#ifndef WIN32

#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

namespace Network
{
    struct SocketApiData
    {
        constexpr SocketApiData(Version t_version,
                                std::string_view t_description,
                                std::string_view t_system_status) :
            m_version(t_version), m_description(t_description),
            m_system_status(t_system_status)
        {
        }

        constexpr SocketApiData() = default;
        constexpr SocketApiData(const SocketApiData&) noexcept = default;
        constexpr SocketApiData(SocketApiData&&) noexcept = default;
        constexpr ~SocketApiData() noexcept = default;
        constexpr auto operator=(const SocketApiData&) noexcept ->
                SocketApiData& = default;
        constexpr auto operator=(SocketApiData&&) noexcept ->
                SocketApiData& = default;

        [[nodiscard]] constexpr auto version() const noexcept -> Version
        {
            return m_version;
        }

        [[nodiscard]] constexpr auto description() const noexcept ->
                std::string_view
        {
            return m_description;
        }

        [[nodiscard]] constexpr auto system_status() const noexcept ->
                std::string_view
        {
            return m_system_status;
        }

    private:
        Version m_version;
        std::string_view m_description;
        std::string_view m_system_status;
    };
}

#endif

#endif
