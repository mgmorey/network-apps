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
#include "network/os-features.h"        // WIN32
#include "network/socket-error.h"       // socket_error

#ifdef WIN32
#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEINPROGRESS,
                            // WSAENETDOWN, WSAEPROCLIM,
                            // WSANOTINITIALISED, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED, WSACleanup(),
                            // WSAStartup()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

#ifdef WIN32
constexpr auto api_default {Network::Version {2, 2}};
#else
constexpr auto api_default {Network::Version {}};
#endif

auto Network::Context::instance() -> const Context&
{
    static Context context;
    return context;
}

Network::Context::Context(const Version& t_version)
{
    const version_type version {t_version ? t_version : api_default};

    try {
#ifdef WIN32
        WSADATA data {};
        const auto error_code {::WSAStartup(version, &data)};

        if (error_code != 0) {
            const auto error_str {format_os_error(error_code)};

            switch (error_code) {  // NOLINT
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
            m_description = data.szDescription;
            m_system_status = data.szSystemStatus;
            m_version = Version {data.wVersion};
            m_is_started = true;
        }
#else
        m_system_status = "Running";
        m_description = "Berkeley Software Distribution Sockets";
        m_version = Version {version};
        m_is_started = true;
#endif

        // Test for class invariant conditions and possibly throw
        // exceptions here if one or more conditions are not met.

        // The following is an example of a test for a trivial
        // invariant, that the API should be initialized (or
        // "Running"):

        if (m_system_status != "Running") {
            throw RuntimeError {"The sockets runtime is not initialized."};
        }
    }
    catch (const Error& error) {
        if (m_is_started && cleanup(failure_mode::return_zero) == 0) {
            m_is_started = false;
        }

        // TODO: Remediate the condition resulting in the following
        // warning from clang-tidy: "thrown exception type is not
        // nothrow copy constructible [cert-err60-cpp]."

        // cppcheck-suppress exceptRethrowCopy
        throw error;  // NOLINT
    }
}

Network::Context::~Context()
{
    shutdown(failure_mode::return_zero);
}

auto Network::Context::description() const noexcept -> std::string
{
    return m_description;
}

auto Network::Context::is_started() const noexcept -> bool
{
    return m_is_started;
}

auto Network::Context::system_status() const noexcept -> std::string
{
    return m_system_status;
}

auto Network::Context::version() const noexcept -> Network::Version
{
    return m_version;
}

auto Network::Context::cleanup(failure_mode t_mode) -> Network::os_error_type
{
#ifdef WIN32
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

auto Network::Context::is_started(bool t_is_started) noexcept -> void
{
    m_is_started = t_is_started;
}

auto Network::Context::shutdown(failure_mode t_mode) -> void
{
    if (m_is_started && cleanup(t_mode) == 0) {
        m_is_started = false;
    }
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) noexcept -> std::ostream&
{
    const auto description {context.description()};
    const auto status {context.system_status()};
    const auto version {context.version()};
    os << description;

    if (version) {
        os << " Version "
           << version;
    }

    os << ' '
       << status
       << std::endl;
    return os;
}
