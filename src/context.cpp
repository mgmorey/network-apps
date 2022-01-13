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
#include "network/os-error.h"           // format_os_error()

#ifdef _WIN32
#include <winsock2.h>   // WSACleanup(), WSAStartup()
#include <windows.h>    // HIBYTE(), LOBYTE()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

#ifdef _WIN32
unsigned Network::Context::m_count;
WSADATA Network::Context::m_data;
#endif

Network::Context::Context(bool t_verbose)
{
#ifdef _WIN32
    if (!m_count++) {
        if (const auto error {::WSAStartup(m_version, &m_data)}) {
            std::ostringstream oss;
            oss << "Call to WSAStartup() returned "
                << error
                << ": "
                << format_os_error(error);
            m_result = {error, oss.str()};
        }
        else if (t_verbose) {
            std::cerr << "Microsoft Windows Sockets Data:"
                      << std::endl
                      << "    Description: "
                      << m_data.szDescription
                      << std::endl
                      << "    System Status: "
                      << m_data.szSystemStatus
                      << std::endl
                      << "    System Version: "
                      << static_cast<int>(LOBYTE(m_data.wVersion))
                      << '.'
                      << static_cast<int>(HIBYTE(m_data.wVersion))
                      << std::endl;
        }
    }
#else
    static_cast<void>(t_verbose);
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

auto Network::Context::result() const -> Network::ErrorResult
{
    return m_result;
}

auto Network::operator<<(std::ostream& os,
                         const Context& context) -> std::ostream&
{
    return os << context.result().string();
}
