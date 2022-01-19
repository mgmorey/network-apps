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
constexpr auto api_default {Network::Version {2, 2}};
#else
constexpr auto api_default {Network::Version {}};
#endif

auto Network::Context::instance() -> Context&
{
    static Context context;
    return context;
}

Network::Context::Context(const Version& t_version)
{
    const auto version {t_version ? t_version : api_default};

    try {
#ifdef _WIN32
        WSADATA data {};
        m_error_code = ::WSAStartup(static_cast<version_type>(version), &data);

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
            m_version = Version {data.wVersion};
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
        if (m_is_started && cleanup(failure_mode::return_zero) == 0) {
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
        cleanup(failure_mode::return_zero);
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

auto Network::Context::cleanup(failure_mode t_mode) -> Network::os_error_type
{
#ifdef _WIN32
    reset_last_os_error();

    if (::WSACleanup() == socket_error) {
        const auto error_code {get_last_os_error()};

        if (t_mode == failure_mode::throw_error) {
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
        else if (t_mode == failure_mode::return_zero) {
            switch (error_code) {  // NOLINT
            case WSANOTINITIALISED:
                return 0;
                break;
            }
        }

        return error_code;
    }

#else
    static_cast<void>(t_mode);
#endif
    return 0;
}

auto Network::Context::shutdown(failure_mode t_mode) -> void
{
    if (m_is_started && cleanup(t_mode) == 0) {
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
