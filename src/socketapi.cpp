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
    if (m_is_started) {
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

auto Network::SocketApi::error_code() const noexcept -> int
{
    return m_error_code;
}

auto Network::SocketApi::is_started() const noexcept -> bool
{
    return m_is_started;
}

auto Network::SocketApi::start() -> void
{
    if (m_is_started) {
        return;
    }

    m_as = Network::start(m_ao);

    if (system_status() != "Running") {
        std::ostringstream oss;
        oss << description()
            << " status is \""
            << system_status()
            << "\".";
        throw RuntimeError {oss.str()};
    }

    m_is_started = true;
}

auto Network::SocketApi::stop() -> void
{
    if (m_is_started) {
        m_error_code = Network::stop(m_ao);

        if (m_error_code == 0) {
            m_is_started = false;
            m_as = {};
        }
    }
}
