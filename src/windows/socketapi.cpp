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

#ifdef WIN32

#include "network/error.hpp"            // Error
#include "network/failmode.hpp"         // FailMode
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/socketapi.hpp"        // SocketApi
#include "network/start.hpp"            // start()
#include "network/stop.hpp"             // stop()
#include "network/version.hpp"          // Version
#include "network/windowsversion.hpp"   // WindowsVersion

#include <sstream>      // std::ostringstream
#include <string>       // std::string
#include <string_view>  // std::string_view

Network::SocketApi::SocketApi(Version t_version,
                              FailMode t_fail_mode,
                              bool t_is_verbose) :
    m_version(t_version),
    m_fail_mode(t_fail_mode),
    m_is_verbose(t_is_verbose)
{
}

Network::SocketApi::SocketApi(FailMode t_fail_mode,
                              bool t_is_verbose) :
    m_fail_mode(t_fail_mode),
    m_is_verbose(t_is_verbose)
{
}

Network::SocketApi::SocketApi(bool t_is_verbose) :
    m_is_verbose(t_is_verbose)
{
}

Network::SocketApi::~SocketApi()
{
    if (m_is_started) {
        Network::stop(m_fail_mode, m_is_verbose);
    }
}

Network::SocketApi::operator std::string() const
{
    std::ostringstream oss;
    oss << m_description
        << " Version "
        << WindowsVersion(m_data.wVersion);

    if (!m_system_status.empty()) {
        oss << ' '
            << m_system_status;
    }

    return oss.str();
}

auto Network::SocketApi::error_code() const noexcept -> int
{
    return m_error_code;
}

auto Network::SocketApi::is_running() const noexcept -> bool
{
    return m_system_status == "Running";
}

auto Network::SocketApi::start() -> Runtime&
{
    if (m_is_started) {
        return *this;
    }

    m_data = m_version ? Network::start(*m_version, m_is_verbose) :
        Network::start(m_is_verbose);
    m_description = static_cast<const char*>(m_data.szDescription);
    m_system_status = static_cast<const char*>(m_data.szSystemStatus);

    if (!is_running()) {
        std::ostringstream oss;
        oss << "SocketApi runtime status is \""
            << m_system_status
            << "\".";
        throw RuntimeError {oss.str()};
    }

    m_is_started = true;
    return *this;
}

auto Network::SocketApi::stop() -> Runtime&
{
    if (m_is_started) {
        m_error_code = Network::stop(m_fail_mode, m_is_verbose);

        if (m_error_code == 0) {
            m_is_started = false;
            m_data = {};
        }
    }

    return *this;
}

#endif
