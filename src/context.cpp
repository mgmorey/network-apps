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
                                        // operator<<(),
                                        // os_error_type, std::ostream
#include "network/error-type.h"         // error_type
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()
#include "network/socket-error.h"       // socket_error

#ifdef _WIN32
#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEINPROGRESS,
                            // WSAENETDOWN, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED, WSACleanup(),
                            // WSAStartup()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

#ifdef _WIN32
static const auto default_version {Network::Version {2, 2}};
#else
static const auto default_version {Network::version_null};
#endif

auto Network::Context::instance() -> Context&
{
    static Context context;
    return context;
}

Network::Context::Context(const Version& t_version)
{
    const auto version {t_version ? t_version : default_version};

    try {
#ifdef _WIN32
        WSADATA data {};
        m_error_code = ::WSAStartup(version, &data);

        if (m_error_code != 0) {
            const auto error_str {format_os_error(m_error_code)};

            switch (m_error_code) {  // NOLINT
            case WSAEFAULT:
            case WSAVERNOTSUPPORTED:
                throw LogicError {error_str};
                break;
            case WSAEPROCLIM:
            case WSASYSNOTREADY:
                throw RuntimeError {error_str};
                break;
            default:
                throw Error {error_str};
            }
        }
        else {
            m_status = data.szSystemStatus;
            m_system = data.szDescription;
            m_version = data.wVersion;
            m_is_started = true;
        }
#else
        m_status = "Running";
        m_system = "Berkeley Software Distribution Sockets";
        m_version = version;
        m_is_started = true;
#endif

        // Test for class invariant conditions and possibly throw
        // exceptions here if one or more conditions are not met.

        // The following is an example of a test for a trivial
        // invariant, that the API should be initialized (or
        // "Running"):

        if (status() != "Running") {
            throw RuntimeError {"The networking runtime is not initialized."};
        }
    }
    catch (const Error& error) {
        if (m_is_started && cleanup(false) == 0) {
            m_is_started = false;
        }

        // TODO: Remediate the condition resulting in the following
        // warning from clang-tidy: "thrown exception type is not
        // nothrow copy constructible [cert-err60-cpp]."

        // NOLINTNEXTLINE
        throw error;
    }
}

Network::Context::~Context()
{
    if (m_is_started) {
        cleanup(false);
    }
}

auto Network::Context::error() const -> Network::os_error_type
{
    return m_error_code;
}

auto Network::Context::started() const -> bool
{
    return m_is_started;
}

auto Network::Context::status() const -> std::string
{
    return m_status;
}

auto Network::Context::system() const -> std::string
{
    return m_system;
}

auto Network::Context::version() const -> Network::Version
{
    return m_version;
}

auto Network::Context::cleanup(bool enable_throw) -> Network::os_error_type
{
    auto error_code {reset_last_os_error()};
#ifdef _WIN32

    if (::WSACleanup() == socket_error) {
        if (enable_throw) {
            error_code = get_last_os_error();
            const auto error_str {format_os_error(error_code)};

            switch (error_code) {  // NOLINT
            case WSANOTINITIALISED:
                throw LogicError {error_str};
                break;
            case WSAEINPROGRESS:
            case WSAENETDOWN:
                throw RuntimeError {error_str};
                break;
            default:
                throw Error {error_str};
            }
        }
        else {
            switch (error_code) {  // NOLINT
            case WSANOTINITIALISED:
                error_code = 0;
                break;
            }
        }
    }

#else
    static_cast<void>(enable_throw);
#endif
    return error_code;
}

auto Network::Context::shutdown() -> void
{
    if (m_is_started && cleanup(true) == 0) {
        m_is_started = false;
    }
}

auto Network::Context::started(bool t_is_started) -> void
{
    m_is_started = t_is_started;
}

auto Network::Context::status(const std::string& t_status) -> void
{
    m_status = t_status;
}

auto Network::Context::system(const std::string& t_system) -> void
{
    m_system = t_system;
}

auto Network::Context::version(const Version& t_version) -> void
{
    m_version = t_version;
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    const auto status {context.status()};
    const auto system {context.system()};
    const auto version {context.version()};
    os << system;

    if (version) {
        os << " Version "
           << version;
    }

    os << ' '
       << status
       << std::endl;
    return os;
}
