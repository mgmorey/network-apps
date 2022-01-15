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
                                        // operator<<(), std::ostream
#include "network/exceptions.h"         // Error, LogicError,
                                        // RunTimeError
#include "network/format-os-error.h"    // format_os_error()

#ifdef _WIN32
#include <winsock2.h>       // WSACleanup(), WSAStartup(), WSAEFAULT,
                            // WSAEPROCLIM, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD()
#endif

#include <cstring>      // std::memset()
#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

#ifdef _WIN32
#define VERSION_DEFAULT	(MAKEWORD(2, 2))	// NOLINT
#else
#define VERSION_DEFAULT	(0)			// NOLINT
#endif

constexpr Network::Context::version_type version_default {VERSION_DEFAULT};

#ifdef _WIN32
unsigned Network::Context::m_count;
WSADATA Network::Context::m_data;
#endif

Network::Context::Context(const OptionalVersion& t_version)
{
    try {
        if ((m_error_code = create(t_version)) != 0) {
            dispatch(m_error_code);
        }

        if (status_string() != "Running") {
            throw RuntimeError("Network is down");
        }
    }
    catch (Network::Error& error) {
        destroy(m_error_code);
        throw error;
    }
}

Network::Context::~Context()
{
    destroy(m_error_code);
}

auto Network::Context::create(const OptionalVersion& t_version) -> error_type
{
    error_type error_code {0};
    const version_type version {t_version ? *t_version : version_default};

#ifdef _WIN32
    if (!m_count++) {
        error_code = ::WSAStartup(version, &m_data);
    }
#else
    static_cast<void>(version);
#endif
    return error_code;
}

auto Network::Context::destroy(error_type error_code) -> void
{
#ifdef _WIN32
    if (!--m_count) {
        if (error_code == 0) {
            ::WSACleanup();
            std::memset(&m_data, '\0', sizeof m_data);
        }
    }
#else
    static_cast<void>(error_code);
#endif
}

auto Network::Context::dispatch(error_type error_code) -> void
{
    const auto what {format_os_error(error_code)};

    switch (error_code) {
    case 0:
        break;
#ifdef _WIN32
    case WSAEFAULT:
    case WSAVERNOTSUPPORTED:
        throw LogicError(what);
        break;
    case WSAEPROCLIM:
    case WSASYSNOTREADY:
        throw RuntimeError(what);
        break;
#endif
    default:
        throw Error(what);
    }
}

auto Network::Context::status_string() -> std::string
{
#ifdef _WIN32
    return m_data.szSystemStatus;
#else
    return "Running";
#endif
}

auto Network::Context::system_string() -> std::string
{
#ifdef _WIN32
    return m_data.szDescription;
#else
    return "";
#endif
}

auto Network::Context::version_number() -> Network::Context::version_type
{
#ifdef _WIN32
    return m_data.wVersion;
#else
    return 0;
#endif
}

auto Network::Context::version_string() -> std::string
{
#ifdef _WIN32
    const version_type version {version_number()};
    const unsigned major {LOBYTE(version)};
    const unsigned minor {HIBYTE(version)};
#else
    const unsigned major {0};
    const unsigned minor {0};
#endif
    return (std::to_string(major) + "." +
            std::to_string(minor));
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
#ifdef _WIN32
    os << "Microsoft Windows Sockets Data:"
       << std::endl
       << "    Description: "
       << context.system_string()
       << std::endl
       << "    System Status: "
       << context.status_string()
       << std::endl
       << "    System Version: "
       << context.version_string()
       << std::endl;
#else
    static_cast<void>(context);
#endif
    return os;
}
