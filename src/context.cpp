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

#include "network/context.h"            // Context, OsError,
                                        // OsErrorResult,
                                        // operator<<(), std::ostream
#include "network/os-error.h"           // format_os_error(),
                                        // get_last_os_error(),
                                        // reset_last_os_error()

#ifdef _WIN32
#include <winsock2.h>       // WSACleanup(), WSAStartup()
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

#ifdef _WIN32
#define WSA_VERSION	(MAKEWORD(2, 2))	// NOLINT
#else
#define WSA_VERSION	(0)			// NOLINT
#endif

#ifdef _WIN32
unsigned Network::Context::m_count;
WSADATA Network::Context::m_data;
#endif

Network::Context::Context(const OptionalVersion& t_version)
{
#ifdef _WIN32
    if (!m_count++) {
        reset_last_os_error();
        const version_type version = t_version ? *t_version : WSA_VERSION;
        const auto code {::WSAStartup(version, &m_data)};

        if (code != 0) {
            const auto error = get_last_os_error();
            std::ostringstream oss;
            oss << "Call to WSAStartup() returned "
                << code
                << ": "
                << format_os_error(error);
            m_result = {error, oss.str()};
        }
    }
#else
    static_cast<void>(t_version);
#endif
}

Network::Context::~Context()
{
#ifdef _WIN32
    if (!--m_count) {
        ::WSACleanup();
    }
#else
    static_cast<void>(m_result);
#endif
}

Network::Context::operator bool() const
{
    return m_result == 0;
}

auto Network::Context::result() const -> Network::OsErrorResult
{
    return m_result;
}

auto Network::Context::version() const -> Network::Context::version_type
{
#ifdef _WIN32
    return m_data.wVersion;
#else
    return 0;
#endif
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
#ifdef _WIN32
    const auto& data = context.m_data;
    os << "Microsoft Windows Sockets Data:"
       << std::endl
       << "    Description: "
       << data.szDescription
       << std::endl
       << "    System Status: "
       << data.szSystemStatus
       << std::endl
       << "    System Version: "
       << static_cast<int>(LOBYTE(data.wVersion))
       << '.'
       << static_cast<int>(HIBYTE(data.wVersion))
       << std::endl;
#else
    static_cast<void>(context);
#endif
    return os;
}
