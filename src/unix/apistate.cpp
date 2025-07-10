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

#ifndef _WIN32

#include "network/apistate.hpp"         // ApiState
#include "network/version.hpp"          // Version

#include <string_view>  // std::string_view

auto Network::ApiState::version() const noexcept -> Version
{
    return m_version;
}

auto Network::ApiState::high_version() const noexcept -> Version
{
    return m_high_version;
}

auto Network::ApiState::description() const noexcept -> std::string_view
{
    return m_description;
}

auto Network::ApiState::system_status() const noexcept -> std::string_view
{
    return m_system_status;
}

#endif
