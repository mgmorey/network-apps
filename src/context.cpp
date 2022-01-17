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
#include <winsock2.h>       // WSACleanup(), WSAStartup(), WSAEFAULT,
                            // WSAEPROCLIM, WSANOTINITIALISED,
                            // WSASYSNOTREADY, WSAVERNOTSUPPORTED
#include <windows.h>        // HIBYTE(), LOBYTE(), MAKEWORD()
#endif

#include <iostream>     // std::cerr, std::endl
#include <sstream>      // std::ostringstream

Network::Context::Context(const OptionalVersion& t_version)
{
#ifdef _WIN32
    try {
        const auto version {t_version ? *t_version : Version {2, 2}};
        m_error_code = ::WSAStartup(version, &m_data);

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
    }
    catch (const Network::Error& error) {
        if (m_error_code == 0) {
            m_error_code = cleanup();
        }

        // Warning from clang-tidy: thrown exception type is not
        // nothrow copy constructible [cert-err60-cpp]

        // NOLINTNEXTLINE
        throw error;
    }
#else
    static_cast<void>(t_version);
#endif
    m_data_dirty = true;
}

Network::Context::~Context()
{
    if (m_data_dirty) {
        static_cast<void>(destroy());
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

auto Network::Context::cleanup(bool verbose) -> Network::os_error_type
{
#ifdef _WIN32
    reset_last_os_error();

    if (::WSACleanup() == socket_error) {
        const auto error_code {get_last_os_error()};

        if (verbose) {
            const auto error_str {format_os_error(error_code)};
            std::cerr << "Call to WSACleanup() failed with error "
                      << error_code
                      << (error_str.empty() ? "" : ": " + error_str)
                      << std::endl;
        }

        return error_code;
    }

#else
    static_cast<void>(verbose);
#endif
    return 0;
}

auto Network::Context::destroy(bool verbose) -> Network::os_error_type
{
    m_error_code = cleanup(verbose);
    m_data_dirty = false;
    return m_error_code;
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
