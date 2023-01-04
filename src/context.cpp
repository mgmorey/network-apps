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

#include "network/context.h"            // Context, Version,
                                        // operator<<(), std::endl,
                                        // std::ostream, std::string
#include "network/cleanup.h"            // cleanup()
#include "network/exceptions.h"         // Error, RuntimeError
#include "network/is-running.h"         // is_running()
#include "network/startup.h"            // startup()

auto Network::Context::instance() -> const Context&
{
    static const Context context;
    return context;
}

Network::Context::Context(const OptionalVersion& t_version)
{
    try {
        startup(*this, t_version);

        // Test for class invariant conditions and possibly throw
        // exceptions here if one or more conditions are not met.

        // The following is an example of a test for a trivial
        // invariant, that the API should be initialized (or
        // "Running"):

        if (!is_running(*this)) {
            throw RuntimeError {"The sockets runtime is not initialized."};
        }
    }
    catch (const Error& error) {
        if (m_is_started && cleanup(failure_mode::return_zero) == 0) {
            m_is_started = false;
        }

        throw;
    }
}

Network::Context::~Context()
{
    shutdown(failure_mode::return_zero);
}

auto Network::Context::description(const std::string& t_description) ->
    Context&
{
    m_description = t_description;
    return *this;
}

auto Network::Context::is_started(bool t_is_started) noexcept -> Context&
{
    m_is_started = t_is_started;
    return *this;
}

auto Network::Context::system_status(const std::string& t_status) ->
    Context&
{
    m_system_status = t_status;
    return *this;
}

auto Network::Context::version(const Version& t_version) noexcept -> Context&
{
    m_version = t_version;
    return *this;
}

auto Network::Context::description() const -> const std::string&
{
    return m_description;
}

auto Network::Context::is_started() const noexcept -> bool
{
    return m_is_started;
}

auto Network::Context::system_status() const -> const std::string&
{
    return m_system_status;
}

auto Network::Context::version() const noexcept -> Network::Version
{
    return m_version;
}

auto Network::Context::shutdown(failure_mode t_mode) -> void
{
    if (m_is_started && cleanup(t_mode) == 0) {
        m_is_started = false;
    }
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    const auto& description {context.description()};
    const auto& status {context.system_status()};
    const auto version {context.version()};
    os << description;

    os << " Version "
       << version;

    if (!status.empty()) {
        os << ' '
           << status;
    }

    os << std::endl;
    return os;
}
