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

#include "network/socketapi.hpp"        // SocketApi
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/start.hpp"            // start()
#include "network/stop.hpp"             // stop()
#include "network/version.hpp"          // Version

#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

Network::SocketApi::SocketApi(ApiOptions t_ao) : m_ao(t_ao)
{
}

Network::SocketApi::~SocketApi()
{
    if (is_running()) {
        Network::stop(m_ao);
    }
}

auto Network::SocketApi::version() const noexcept -> Version
{
    return m_as.version();
}

auto Network::SocketApi::high_version() const noexcept -> Version
{
    return m_as.high_version();
}

auto Network::SocketApi::description() const noexcept -> std::string_view
{
    return m_as.description();
}

auto Network::SocketApi::system_status() const noexcept -> std::string_view
{
    return m_as.system_status();
}

auto Network::SocketApi::is_running() const noexcept -> bool
{
    return m_as.system_status() == "Running";
}

auto Network::SocketApi::is_verbose() const noexcept -> bool
{
    return m_ao.is_verbose();
}

auto Network::SocketApi::start() -> void
{
    if (is_running()) {
        return;
    }

    m_as = Network::start(m_ao);

    if (!is_running()) {
        std::ostringstream oss;
        oss << m_as.description()
            << " status is \""
            << m_as.system_status()
            << "\".";
        throw RuntimeError {oss.str()};
    }
}

auto Network::SocketApi::stop() -> int
{
    int error_code {0};

    if (is_running()) {
        error_code = Network::stop(m_ao);

        if (error_code == 0) {
            m_as = {};
        }
    }

    return error_code;
}
