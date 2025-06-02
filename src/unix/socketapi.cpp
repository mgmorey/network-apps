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
#include "network/is-running.hpp"       // is_running()
#include "network/optionalversion.hpp"  // OptionalVersion
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/start.hpp"            // start()
#include "network/stop.hpp"             // stop()

#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view

Network::SocketApi::SocketApi(const RuntimeData& t_rd) : m_rt_data(t_rd)
{
}

Network::SocketApi::~SocketApi()
{
    if (m_rt_state.m_is_started) {
        Network::stop(m_rt_data.fail_mode(), m_rt_data.is_verbose());
    }
}

auto Network::SocketApi::description() const noexcept -> std::string_view
{
    return m_sa_data.m_description;
}

auto Network::SocketApi::error_code() const noexcept -> int
{
    return m_rt_state.m_error_code;
}

auto Network::SocketApi::start() -> void
{
    if (m_rt_state.m_is_started) {
        return;
    }

    m_sa_data = Network::start(m_rt_data.is_verbose());

    if (!is_running(*this)) {
        std::ostringstream oss;
        oss << "BSD Socket API runtime status is \""
            << system_status()
            << "\".";
        throw RuntimeError {oss.str()};
    }

    m_rt_state.m_is_started = true;
}

auto Network::SocketApi::stop() -> void
{
    if (m_rt_state.m_is_started) {
        m_rt_state.m_error_code = Network::stop(m_rt_data.fail_mode(),
                                                m_rt_data.is_verbose());

        if (m_rt_state.m_error_code == 0) {
            m_rt_state.m_is_started = false;
            m_sa_data = {};
        }
    }
}

auto Network::SocketApi::system_status() const noexcept -> std::string_view
{
    return m_sa_data.m_system_status;
}

auto Network::SocketApi::version() const noexcept -> OptionalVersion
{
    return m_sa_data.m_version;
}

#endif
