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

#include "network/windowscontext.h"     // WindowsContext
#include "network/error.h"              // Error
#include "network/failuremode.h"        // FailureMode
#include "network/optionalversion.h"    // OptionalVersion
#include "network/runtimeerror.h"       // RuntimeError
#include "network/start.h"              // start()
#include "network/stop.h"               // stop()

#include <sstream>      // std::ostringstream

Network::WindowsContext::WindowsContext(const OptionalVersion& t_version,
                                        FailureMode t_failure,
                                        bool t_is_verbose)
    : m_version(t_version),
      m_failure(t_failure),
      m_is_verbose(t_is_verbose)
{
}

Network::WindowsContext::WindowsContext(bool t_is_verbose)
    : m_is_verbose(t_is_verbose)
{
}

Network::WindowsContext::~WindowsContext()
{
    shut_down();
}

auto Network::WindowsContext::error_code() const noexcept -> int
{
    return m_error_code;
}

auto Network::WindowsContext::is_running() const noexcept -> bool
{
    return m_is_started && system_status() == "Running";
}

auto Network::WindowsContext::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::WindowsContext::start() -> Context*
{
    if (!m_is_started) {
        m_data = Network::start(m_version, m_is_verbose);
        m_is_started = true;
    }

    return this;
}

auto Network::WindowsContext::start_up() -> Context*
{
    try {
        start();

        if (!is_running()) {
            throw RuntimeError
            {
                "Sockets runtime status is \"" + system_status() + "\"."
            };
        }
    }
    catch (const Error& error) {
        stop();
        throw;
    }

    return this;
}

auto Network::WindowsContext::stop() -> Context*
{
    if (m_is_started) {
        m_error_code = Network::stop(m_failure, m_is_verbose);

        if (m_error_code == 0) {
            m_is_started = false;
            m_data = {};
        }
    }

    return this;
}

auto Network::WindowsContext::shut_down() const -> void
{
    if (m_is_started) {
        Network::stop(FailureMode::return_zero, m_is_verbose);
    }
}

auto Network::WindowsContext::to_string() const -> std::string
{
    const auto& description {m_data.m_description};
    const auto& status {m_data.m_system_status};
    const auto version {m_data.m_version};
    std::ostringstream oss;
    oss << description;

    if (version) {
        oss << " Version "
            << *version;
    }

    if (!status.empty()) {
        oss << ' '
            << status;
    }

    return oss.str();
}