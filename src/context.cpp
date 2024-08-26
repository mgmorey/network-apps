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
#include "network/is-running.h"         // is_running()
#include "network/optionalversion.h"    // OptionalVersion
#include "network/runtimeerror.h"       // RuntimeError
#include "network/start.h"              // start()
#include "network/stop.h"               // stop()

auto Network::Context::instance() -> const Context&
{
    static const Context context;
    return context;
}

Network::Context::Context(const OptionalVersion& t_version, bool t_is_verbose)
    : m_is_verbose(t_is_verbose)
{
    try {
        start(t_version);

        // Test for class invariant conditions and throw exceptions if
        // one or more conditions are not met. The following is an
        // example of a test for a trivial invariant (that the API
        // should be initialized or "running"):

        if (!is_running(*this)) {
            throw RuntimeError {"The sockets runtime is not initialized."};
        }
    }
    catch (const Error& error) {
        stop(failure_mode::return_zero);
        throw;
    }
}

Network::Context::~Context()
{
    stop(failure_mode::return_zero);
}

auto Network::Context::error_code() const noexcept -> context_error_type
{
    return m_error_code;
}

auto Network::Context::is_verbose() const noexcept -> bool
{
    return m_is_verbose;
}

auto Network::Context::start(const OptionalVersion& t_version) -> Context&
{
    if (m_is_started) {
        return *this;
    }

    return Network::start(*this, t_version);
}

auto Network::Context::stop(failure_mode mode) -> Context&
{
    if (m_is_started) {
        m_error_code = Network::stop(mode, m_is_verbose);

        if (m_error_code == 0) {
            m_is_started = false;
        }
    }

    return *this;
}
