// Copyright (C) 2024  "Michael G. Morey" <mgmorey@gmail.com>

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

#ifdef WIN32

#include "network/start.hpp"            // start()
#include "network/contextdata.hpp"      // ContextData
#include "network/error.hpp"            // Error
#include "network/format-os-error.hpp"  // format_os_error()
#include "network/logicerror.hpp"       // LogicError
#include "network/optionalversion.hpp"  // OptionalVersion
#include "network/runtimeerror.hpp"     // RuntimeError
#include "network/to-os-error.hpp"      // to_os_error()
#include "network/version.hpp"          // Version
#include "network/windowsversion.hpp"   // WindowsVersion

#include <winsock2.h>       // WSAEFAULT, WSAEPROCLIM, WSASYSNOTREADY,
                            // WSAVERNOTSUPPORTED, ::WSAStartup()

#include <iostream>     // std::cout, std::endl

namespace {
    auto start(Network::OptionalVersion version,
               bool is_verbose) -> Network::ContextData
    {
        ContextData wsa_data {};
        const WindowsVersion wsa_version
        {
            version.value_or(WindowsVersion::latest)
        };

        if (is_verbose) {
            // clang-format off
            std::cout << "Starting the network runtime.\n"
                      << "Calling ::WSAStartup("
                      << wsa_version
                      << ", ...)"
                      << std::endl;
            // clang-format on
        }

        if (const auto api_error {::WSAStartup(wsa_version, &wsa_data)}) {
            const auto os_error {to_os_error(api_error)};
            const auto message {format_os_error(os_error)};

            if (is_verbose) {
                // clang-format off
                std::cout << "Call to ::WSAStartup("
                          << wsa_version
                          << ", ...) failed with error "
                          << api_error
                          << ": "
                          << message
                          << std::endl;
                // clang-format on
            }

            switch (api_error) {  // NOLINT
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

        return wsa_data;
    }
} // namespace

auto Network::start([[maybe_unused]] Version version,
                    bool is_verbose) -> ContextData
{
    return ::start(version, is_verbose);
}

auto Network::start(bool is_verbose) -> ContextData
{
    return ::start({}, is_verbose);
}

#endif
