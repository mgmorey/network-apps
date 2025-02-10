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

#include "network/nativecontext.h"      // NativeContext
#include "network/failuremode.h"        // FailureMode
#include "network/optionalversion.h"    // OptionalVersion
#include "network/runtimeerror.h"       // RuntimeError
#include "network/start.h"              // start()
#include "network/stop.h"               // stop()

#include <sstream>      // std::ostringstream
#include <string>       // std::string

Network::NativeContext::NativeContext(const OptionalVersion& t_version,
                                      FailureMode t_failure,
                                      bool t_is_verbose)
    : m_version(t_version),
      m_is_verbose(t_is_verbose)
{
    static_cast<void>(t_failure);
}

Network::NativeContext::NativeContext(bool t_is_verbose)
    : m_is_verbose(t_is_verbose)
{
}

Network::NativeContext::~NativeContext()
{
    if (m_is_started) {
        Network::stop(FailureMode::return_zero, m_is_verbose);
    }
}

Network::NativeContext::operator std::string() const
{
    std::ostringstream oss;
    oss << m_data.m_description;

    if (m_data.m_version) {
        oss << " Version "
            << *m_data.m_version;
    }

    if (!m_data.m_system_status.empty()) {
        oss << ' '
            << m_data.m_system_status;
    }

    return oss.str();
}

auto Network::NativeContext::error_code() const noexcept -> int
{
    return m_error_code;
}

auto Network::NativeContext::is_running() const noexcept -> bool
{
    return m_data.m_system_status == "Running";
}

auto Network::NativeContext::start() -> Context&
{
    if (m_is_started) {
        return *this;
    }

    m_data = Network::start(m_version, m_is_verbose);
    m_is_started = true;

    if (!is_running()) {
        std::ostringstream oss;
        oss << "Sockets runtime status is \""
            << m_data.m_system_status
            << "\".";
        throw RuntimeError {oss.str()};
    }

    m_is_started = true;
    return *this;
}

auto Network::NativeContext::stop() -> Context&
{
    if (m_is_started) {
        m_error_code = Network::stop(FailureMode::return_zero, m_is_verbose);

        if (m_error_code == 0) {
            m_is_started = false;
            m_data = {};
        }
    }

    return *this;
}

auto Network::NativeContext::version() const -> OptionalVersion
{
    return m_data.m_version;
}

#endif
