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

#ifndef WIN32

#include "network/socketapi.hpp"        // SocketApi
#include "network/failmode.hpp"         // FailMode
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/start.hpp"            // start()
#include "network/stop.hpp"             // stop()

#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::SocketApi::SocketApi(FailMode t_fail_mode,
                              bool t_is_verbose)
{
    m_state.m_fail_mode = t_fail_mode;
    m_state.m_is_verbose = t_is_verbose;
}

Network::SocketApi::SocketApi(bool t_is_verbose)
{
    m_state.m_is_verbose = t_is_verbose;
}

Network::SocketApi::~SocketApi()
{
    if (m_state.m_is_started) {
        Network::stop(FailMode::return_zero, m_state.m_is_verbose);
    }
}

Network::SocketApi::operator std::string() const
{
    std::ostringstream oss;
    oss << m_data.m_description;

    if (!m_data.m_system_status.empty()) {
        oss << ' '
            << m_data.m_system_status;
    }

    return oss.str();
}

auto Network::SocketApi::error_code() const noexcept -> int
{
    return m_state.m_error_code;
}

auto Network::SocketApi::is_running() const noexcept -> bool
{
    return m_data.m_system_status == "Running";
}

auto Network::SocketApi::start() -> Runtime&
{
    if (m_state.m_is_started) {
        return *this;
    }

    m_data = Network::start(m_state.m_is_verbose);

    if (!is_running()) {
        std::ostringstream oss;
        oss << "Socket API runtime status is \""
            << m_data.m_system_status
            << "\".";
        throw RuntimeError {oss.str()};
    }

    m_state.m_is_started = true;
    return *this;
}

auto Network::SocketApi::stop() -> Runtime&
{
    if (m_state.m_is_started) {
        m_state.m_error_code = Network::stop(m_state.m_fail_mode,
                                             m_state.m_is_verbose);

        if (m_state.m_error_code == 0) {
            m_state.m_is_started = false;
            m_data = {};
        }
    }

    return *this;
}

#endif
