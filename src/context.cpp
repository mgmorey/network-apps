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

#include "network/context.h"            // Context, ErrorResult,
                                        // Version, operator<<(),
                                        // std::ostream
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/format-os-error.h"    // format_os_error()

#ifdef _WIN32
#include <winsock2.h>       // WSACleanup(), WSAStartup(), WSAEFAULT,
                            // WSAEPROCLIM, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

constexpr auto status_running {"Running"};
constexpr auto system_bsdsock {"Berkeley Software Distribution Sockets"};
constexpr auto version_radix {256U};

Network::Context::Context(const OptionalVersion& t_version)
{
#ifdef _WIN32
    const auto default_version {version(2, 2)};
#else
    const auto default_version {version(0, 0)};
#endif

    try {
        const auto request_version {t_version ? *t_version : default_version};
#ifdef _WIN32
        m_error_code = ::WSAStartup(request_version, &m_data);
#else
        m_data.m_status = status_running;
        m_data.m_system = system_bsdsock;
        m_data.m_version = request_version;
#endif

        if (m_error_code != 0) {
            dispatch(m_error_code);
        }
    }
    catch (const Network::Error& error) {
        cleanup(m_error_code);
        // warning: thrown exception type is not nothrow copy
        // constructible [cert-err60-cpp]
        // NOLINTNEXTLINE
        throw error;
    }
}

Network::Context::~Context()
{
    cleanup(m_error_code);
}

auto Network::Context::status_string() const -> std::string
{
#ifdef _WIN32
    return m_data.szSystemStatus;
#else
    return m_data.m_status;
#endif
}

auto Network::Context::system_string() const -> std::string
{
#ifdef _WIN32
    return m_data.szDescription;
#else
    return m_data.m_system;
#endif
}

auto Network::Context::version_number() const -> Network::Version
{
#ifdef _WIN32
    return m_data.wVersion;
#else
    return m_data.m_version;
#endif
}

auto Network::Context::version_string() const -> std::string
{
    return version_string(version_number());
}

auto Network::Context::cleanup(error_type error_code) -> void
{
    if (error_code == 0) {
#ifdef _WIN32
        if ((error_code = ::WSACleanup()) != 0 ) {
            std::cerr << format_os_error(error_code)
                      << std::endl;
        }
#endif
    }
}

auto Network::Context::dispatch(error_type error_code) -> void
{
    const auto error_str {format_os_error(error_code)};

    switch (error_code) {  // NOLINT
#ifdef _WIN32
    case WSAEFAULT:
    case WSAVERNOTSUPPORTED:
        throw LogicError {error_str};
        break;
    case WSAEPROCLIM:
    case WSASYSNOTREADY:
        throw RuntimeError {error_str};
        break;
#endif
    default:
        throw Error {error_str};
    }
}

auto Network::Context::version(const Version& major,
                               const Version& minor) -> Network::Version
{
    return minor * version_radix + major;
}

auto Network::Context::version_string(const Version& version) -> std::string
{
    const auto major = version % version_radix;
    const auto minor = version / version_radix;
    return (std::to_string(major) + "." +
            std::to_string(minor));
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    const auto status {context.status_string()};
    const auto system {context.system_string()};
    const auto version {context.version_string()};
    os << system;

    if (version != "0.0") {
        os << " Version "
           << version;
    }

    os << ' '
       << status
       << std::endl;
    return os;
}
