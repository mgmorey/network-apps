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
    const Version default_version {2, 2};
#else
    const Version default_version {0, 0};
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
    m_error_code = cleanup(m_error_code);

    if (m_error_code != 0) {
        std::cerr << format_os_error(m_error_code)
                  << std::endl;
    }
}

auto Network::Context::status() const -> std::string
{
#ifdef _WIN32
    return m_data.szSystemStatus;
#else
    return m_data.m_status;
#endif
}

auto Network::Context::system() const -> std::string
{
#ifdef _WIN32
    return m_data.szDescription;
#else
    return m_data.m_system;
#endif
}

auto Network::Context::version() const -> Network::Version
{
#ifdef _WIN32
    return m_data.wVersion;
#else
    return m_data.m_version;
#endif
}

auto Network::Context::cleanup(error_type error_code) -> Network::error_type
{
#ifdef _WIN32
    if (error_code == 0) {
        error_code = ::WSACleanup();
    }
#endif

    return error_code;
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

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    const auto status {context.status()};
    const auto system {context.system()};
    const auto version {context.version()};
    os << system;

    if (version != 0) {
        os << " Version "
           << version;
    }

    os << ' '
       << status
       << std::endl;
    return os;
}
