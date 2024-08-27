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

#include "network/start.h"              // start()
#include "network/contextdata.h"        // ContextData
#include "network/optionalversion.h"    // OptionalVersion

#ifdef WIN32
#include "network/error.h"              // Error
#include "network/format-os-error.h"    // format_os_error()
#include "network/logicerror.h"         // LogicError
#include "network/runtimeerror.h"       // RuntimeError
#include "network/to-os-error.h"        // to_os_error()
#include "network/version.h"            // Version
#include "network/windowsversion.h"     // WindowsVersion
#endif

#ifdef WIN32
#include <winsock2.h>       // WSADATA, WSAEFAULT, WSAEPROCLIM,
                            // WSASYSNOTREADY, WSAVERNOTSUPPORTED,
                            // ::WSAStartup()
#include <windows.h>        // WORD
#endif

#ifdef WIN32
#include <iostream>     // std::cerr, std::cout, std::endl
#include <sstream>      // std::ostringstream
#include <string_view>  // std::string_view
#endif

#ifdef WIN32
static constexpr Network::Version wsa_default {2, 2};
#else
static constexpr auto description {
    "Berkeley Software Distribution Sockets"
};
static constexpr auto system_running {
    "Running"
};
#endif

auto Network::start(const OptionalVersion& version,
                    bool is_verbose) -> ContextData
{
#ifdef WIN32
    const WindowsVersion wsa_version {version.value_or(wsa_default)};
    WSADATA wsa_data {};

    if (is_verbose) {
        std::cout << "Calling ::WSAStartup("
                  << wsa_version
                  << ", ...)"
                  << std::endl;
    }

    if (const auto error {::WSAStartup(WORD {wsa_version}, &wsa_data)}) {
        const auto os_error {to_os_error(error)};
        const auto message {format_os_error(os_error)};

        if (is_verbose) {
            std::cerr << "Call to ::WSAStartup("
                      << wsa_version
                      << ", ...) failed with error "
                      << error
                      << ": "
                      << message
                      << std::endl;
        }

        switch (error) {  // NOLINT
        case WSAEFAULT:
        case WSAVERNOTSUPPORTED:
            throw LogicError {message};
        case WSAEPROCLIM:
        case WSASYSNOTREADY:
            throw RuntimeError {message};
        default:
            throw Error {message};
        }
    }

    return {std::string_view(wsa_data.szDescription),
            std::string_view(wsa_data.szSystemStatus),
            WindowsVersion {wsa_data.wVersion}};
#else
    static_cast<void>(is_verbose);
    return {description,
            system_running,
            version};
#endif
}
