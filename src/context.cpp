// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

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

#include "network/context.h"            // Context
#include "network/error.h"              // Error
#include "network/failuremode.h"        // FailureMode
#include "network/optionalversion.h"    // OptionalVersion
#include "network/runtimeerror.h"       // RuntimeError
#include "network/start.h"              // start()
#include "network/stop.h"               // stop()

Network::Context::Context(const OptionalVersion& t_version, bool t_is_verbose)
    : m_is_verbose(t_is_verbose)
{
    try {
        start(t_version);

        // Test for class invariant conditions and throw exceptions if
        // one or more conditions are not met. The following is an
        // example of a test for a trivial invariant (that the API
        // should be initialized or "running"):

        if (!is_running()) {
            throw RuntimeError {"The sockets runtime is not initialized."};
        }
    }
    catch (const Error& error) {
        stop(m_failsafe);
        throw;
    }
}

Network::Context::~Context()
{
    if (m_is_started) {
        Network::stop(m_failsafe, m_is_verbose);
    }
}

auto Network::Context::error_code() const noexcept -> int
{
    return m_error_code;
}

auto Network::Context::is_running() const noexcept -> bool
{
    return m_is_started && m_data.m_system_status == "Running";
}

auto Network::Context::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::Context::start(const OptionalVersion& t_version) -> Context&
{
    if (!m_is_started) {
        m_data = Network::start(t_version, m_is_verbose);
        m_is_started = true;
    }

    return *this;
}

auto Network::Context::stop(FailureMode t_mode) -> Context&
{
    if (m_is_started) {
        m_error_code = Network::stop(t_mode, m_is_verbose);

        if (m_error_code == 0) {
            m_is_started = false;
            m_data = {};
        }
    }

    return *this;
}
